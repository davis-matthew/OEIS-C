/*
 * A055446  Base 6 distribution of first digit of mantissa following Benford's
 *          Law, to minimize chi-squared statistic.
 * https://oeis.org/A055446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = digit that minimises the χ²‑statistic between the
 *            observed counts of digits 1‑5 up to n and the Benford
 *            probabilities in base 6. For each n the sequence picks the digit
 *            that yields the smallest χ²; ties are broken by the smallest
 *            digit
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* seq_t and idx_t are supplied by the build system. */
/* seq_t must be constructible from an integer (true for all supplied
   integer types and for the arbitrary‑precision integer class). */
static seq_t *seq = NULL;           /* 1‑based storage, seq[0] unused   */
static size_t seq_cap = 0;          /* allocated capacity (in elements)  */
static idx_t seq_len = 0;           /* largest n that has been computed   */
static idx_t cnt[6] = {0,0,0,0,0,0};/* running counts of digits 1‑5      */

/* Benford probabilities for base 6 (digits 1‑5). */
static const double p[6] = {
    0.0,                           /* unused                         */
    log(2.0)   / log(6.0),         /* d = 1                          */
    log(1.5)   / log(6.0),         /* d = 2                          */
    log(4.0/3.0)/ log(6.0),        /* d = 3                          */
    log(5.0/4.0)/ log(6.0),        /* d = 4                          */
    log(6.0/5.0)/ log(6.0)         /* d = 5                          */
};

/* Ensure the array can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((idx_t)seq_cap > n) return;
    size_t new_cap = seq_cap > 0 ? seq_cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Compute the sequence up to n if needed. */
static void compute_upto(idx_t n)
{
    if (n <= seq_len) return;
    ensure_capacity(n);
    for (idx_t k = seq_len + 1; k <= n; ++k) {
        double exp_cnt[6];
        for (int d = 1; d <= 5; ++d)
            exp_cnt[d] = (double)k * p[d];

        double best_chi = 1e308;
        int best_d = 1;
        for (int d = 1; d <= 5; ++d) {
            double chi = 0.0;
            for (int j = 1; j <= 5; ++j) {
                double val = (double)cnt[j] + (j == d ? 1.0 : 0.0);
                double diff = val - exp_cnt[j];
                chi += diff * diff / exp_cnt[j];
            }
            if (chi < best_chi - 1e-12) {
                best_chi = chi;
                best_d = d;
            } else if (fabs(chi - best_chi) < 1e-12 && d < best_d) {
                best_d = d;   /* tie → smallest digit */
            }
        }
        seq[k] = (seq_t)best_d;
        cnt[best_d] += 1;
    }
    seq_len = n;
}

seq_t a(idx_t n)
{
    compute_upto(n);
    return seq[n];
}

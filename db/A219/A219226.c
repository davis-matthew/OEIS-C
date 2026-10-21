/*
 * A219226  Number of rooted unlabeled ordered (plane) trees with 2n leaves
 *          such that i) every internal node has an even number of children
 *          and ii) every path from the root to a leaf is the same length.
 * https://oeis.org/A219226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) = coefficient of x^(2n) in the series A(x) satisfying A(x)
 *            = x + A(x^2/(1-x^2)) Recurrence (t = n): a(1) = 1 a(t) =
 *            Σ_{k=1}^{t} a(k) * binom(t-1, k-1) for t ≥ 2 (a(0) = 0). This
 *            gives the required values a(n) = c[2n]
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 27, u64 n <= 50, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static storage for coefficients of A(x) */
static seq_t *coeff = NULL;          /* coefficients c[i] = [x^i]A(x) */
static idx_t max_t_computed = 0;     /* maximum t for which c[2*t] has been computed */
static size_t coeff_size = 0;        /* allocated size of coeff array (indices 0 .. coeff_size-1) */

/* Ensure that coeff has indices up to max_index (inclusive) */
static void ensure_capacity(size_t max_index)
{
    if (max_index < coeff_size) return;
    size_t new_size = max_index + 1;
    seq_t *tmp = (seq_t *)realloc(coeff, new_size * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);          /* allocation failed */
    for (size_t i = coeff_size; i < new_size; ++i) tmp[i] = 0;   /* initialise new slots */
    coeff = tmp;
    coeff_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* a(0) = 0 */

    /* initialise on first use */
    if (!coeff) {
        coeff_size = 3;                   /* indices 0,1,2 */
        coeff = (seq_t *)malloc(coeff_size * sizeof(seq_t));
        if (!coeff) exit(EXIT_FAILURE);
        for (size_t i = 0; i < coeff_size; ++i) coeff[i] = 0;
        coeff[1] = 1;                     /* c[1] = 1  → a(1) = 1 */
        max_t_computed = 0;
    }

    /* allocate space for indices up to 2*n */
    ensure_capacity(2 * (size_t)n);

    /* compute missing coefficients for t = max_t_computed+1 .. n */
    for (idx_t t = max_t_computed + 1; t <= n; ++t) {
        seq_t sum = 0;
        seq_t binom = 1;                  /* C(t-1,0) */
        for (idx_t k = 1; k <= t; ++k) {
            sum += coeff[(size_t)k] * binom;
            if (k < t) {                  /* update to C(t-1,k) */
                seq_t num = (seq_t)(t - k);   /* t-k   */
                seq_t den = (seq_t)k;         /* k     */
                binom = binom * num / den;    /* exact division */
            }
        }
        coeff[2 * (size_t)t] = sum;        /* store c[2*t] */
    }
    max_t_computed = n;

    return coeff[2 * (size_t)n];
}

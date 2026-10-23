/*
 * A100861  Triangle of Bessel numbers read by rows: T(n,k) is the number of
 *          k-matchings of the complete graph K(n).
 * https://oeis.org/A100861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, nice
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 52, u32 n <= 116, u64 n <= 301,
 *            u128 n <= 834, big any n
 * Helpers:   double_fact_k, f_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/double_fact_k.h"
#include "../utils/f_val.h"
/* Triangle of Bessel numbers read by rows (OEIS A100861).
   The n‑th term a(n) is the n‑th entry of the triangle
   T(n,k) = n!/(k!(n-2k)!·2^k) read row‑wise, starting with
   row 0.  The triangle row lengths are floor(r/2)+1. */

/* ---- greatest common divisor for seq_t ---- */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---- binomial coefficient C(n,k) for small idx_t arguments ---- */
static inline seq_t binom_idx_t(idx_t n, idx_t k)
{
    if (k > n)               return (seq_t)0;
    if (k == (idx_t)0 || k == n) return (seq_t)1;
    if (k > n - k)           k = n - k;   /* symmetry */

    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);  /* numerator > 0 */
        seq_t den = (seq_t)i;            /* denominator <= k */

        seq_t g = gcd_seq(num, den);
        num /= g;
        den /= g;

        g = gcd_seq(res, den);
        res /= g;
        den /= g;                       /* den should now be 1 */

        res *= num;                    /* safe: res <= final binomial */
    }
    return res;
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    /* Find row r such that f_val(r-1) ≤ n < f_val(r)  */
    idx_t low = (idx_t)0;
    idx_t high = n;                     /* r never exceeds n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (f_val(mid) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;                      /* row index */

    idx_t prev_f = (r == (idx_t)0) ? (idx_t)0 : f_val(r - 1);
    idx_t k = n - prev_f;               /* position within the row */

    /* Compute T(r,k) = C(r,2k) * (2k-1)!! */
    seq_t bin = binom_idx_t(r, k * 2);
    seq_t df  = double_fact_k(k);
    return bin * df;
}

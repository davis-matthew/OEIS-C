/*
 * A227394  The maximum value of x^4(n-x)(x-1) for x in 1..n is reached for x
 *          = a(n).
 * https://oeis.org/A227394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((10*n + 7)/12) for n > 10, otherwise a(n) is the
 *            argmax of f(x)=x^4*(n-x)*(x-1) for 1 <= x <= n
 * Range:     u8 n <= 4, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* For n <= 10 evaluate the function directly.
       The maximum is unique for all n >= 1, so we simply keep the
       first x that attains the maximum. */
    if (n <= 10) {
        idx_t best_x = 1;
        seq_t best_val = 0;               /* value for x = 1 */

        /* Evaluate f(x) for each candidate x.  32‑bit arithmetic is
           sufficient here because the largest value is < 2^32. */
        for (idx_t x = 1; x <= n; ++x) {
            seq_t xv  = (seq_t)x;
            seq_t val = xv * xv * xv * xv;   /* x⁴   */
            val *= (seq_t)(n - x);
            val *= (seq_t)(x - 1);
            if (x == 1 || val > best_val) {  /* keep the first maximum */
                best_val = val;
                best_x   = x;
            }
        }
        return (seq_t)best_x;
    }

    /* For n > 10 the closed form applies. */
    seq_t n_seq = (seq_t)n;
    return ((seq_t)10 * n_seq + 7) / 12;
}

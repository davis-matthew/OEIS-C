/*
 * A181067  a(n) = Sum_{k=0..n-1} binomial(n-1,k)^2 * binomial(n,k).
 * https://oeis.org/A181067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n-1,k)^2 * binomial(n,k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The argument n is 1‑based; a(1)=1. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour for n<1 is unused */

    seq_t sum = 0;
    seq_t bin_nm1 = 1;   /* C(n-1,0) */
    seq_t bin_n   = 1;   /* C(n,0)   */

    for (idx_t k = 0; k < n; ++k) {
        seq_t bin_sq = bin_nm1 * bin_nm1;          /* C(n-1,k)^2   */
        seq_t term   = bin_sq * bin_n;            /* *C(n,k)      */
        sum += term;

        /* update binomials for the next k+1, skip last iteration */
        if (k + 1 == n) break;

        /* C(n-1,k+1) = C(n-1,k) * (n-1-k)/(k+1)   */
        seq_t tmp = bin_nm1 * (seq_t)(n - 1 - k);
        bin_nm1 = tmp / (seq_t)(k + 1);

        /* C(n,k+1)   = C(n,k)   * (n-k)/(k+1)   */
        tmp = bin_n * (seq_t)(n - k);
        bin_n = tmp / (seq_t)(k + 1);
    }

    return sum;
}

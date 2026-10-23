/*
 * A126453  Row sums of triangle A126450: a(n) = Sum_{k=0..n} C( C(n+2,3) -
 *          C(k+2,3) + 1, n-k).
 * https://oeis.org/A126453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n} C( C(n+2,3) - C(k+2,3) + 1, n-k )
 * (OEIS sequence A126453)
 */

#include <stddef.h>   /* for size_t */

/* Helper: binomial coefficient with idx_t arguments, returning seq_t. */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply first, then divide.  The division is exact. */
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* M = C(n+2,3) = (n+2)(n+1)n/6 */
    seq_t M = ((seq_t)(n + 2)) * (n + 1) * n / 6;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* Ck = C(k+2,3) = (k+2)(k+1)k/6 */
        seq_t Ck = ((seq_t)(k + 2)) * (k + 1) * k / 6;

        /* top argument for the outer binomial */
        seq_t top = M - Ck + 1;

        /* Binomial term C(top, n-k) */
        seq_t term = binom_idx((idx_t)top, n - k);

        sum += term;
    }
    return sum;
}

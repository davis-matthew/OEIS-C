/*
 * A189390  The maximum possible value for the apex of a triangle of numbers
 *          whose base consists of a permutation of the numbers 0 to n, and
 *          each number in a higher row is the sum of the two numbers directly
 *          below it.
 * https://oeis.org/A189390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..⌊(n-1)/2⌋} (4·k+1)·C(n,k) + (n+1 mod
 *            2)·n·C(n,n/2) where C(n,k) is the binomial coefficient
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t result = 0;
    seq_t binom   = 1;                  /* C(n,0) */
    idx_t kmax    = (n - 1) / 2;        /* floor((n-1)/2) */

    for (idx_t k = 0; k <= kmax; ++k) {
        result += binom * (seq_t)(4 * k + 1);
        if (k < kmax)
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
    }

    /* extra term when n is even: (n+1 mod 2)=1  */
    if ((n & 1) == 0) {
        /* C(n,n/2) is the next binomial after C(n,kmax) */
        seq_t mid_binom = binom * (seq_t)(n - kmax) / (seq_t)(kmax + 1);
        result += (seq_t)n * mid_binom;
    }

    return result;
}

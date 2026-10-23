/*
 * A109340  Expansion of x^2*(1+x+4*x^2)/((1+x+x^2)*(1-x)^3).
 * https://oeis.org/A109340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n-1)^2 - (((n+1) mod 3) mod 2) for n ≥ 1, a(0) = 0
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
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

    /* compute (n-1)^2 */
    seq_t nm1 = (seq_t)n - 1;
    seq_t sq  = nm1 * nm1;

    /* compute ((n+1) mod 3) mod 2 */
    uint64_t r  = (n + 1) % 3;
    uint64_t m2 = r % 2;

    return sq - (seq_t)m2;
}

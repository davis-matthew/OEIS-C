/*
 * A195124  2n-floor(n*e/2).
 * https://oeis.org/A195124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - floor(n*e/2)
 * Range:     u8 n <= 397, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) = 2*n - ⌊n*e/2⌋ using integer arithmetic only.  */
seq_t a(idx_t n)
{
    /* Scale e to an integer: e * 10^18 = 2718281828459045236 */
    static const unsigned __int128 SCALE      = 1000000000000000000ULL;   /* 1e18   */
    static const unsigned __int128 E_SCALED  = 2718281828459045236ULL;   /* e*1e18 */

    /* floor(n*e/2) = floor(n * E_SCALED / (2*SCALE)) */
    unsigned __int128 floor_val = ((unsigned __int128)n * E_SCALED) / (2 * SCALE);

    /* 2 * n */
    unsigned __int128 two_n = (unsigned __int128)n * 2;

    /* result in 128‑bit, cast to seq_t */
    unsigned __int128 res128 = two_n - floor_val;
    return (seq_t)res128;
}

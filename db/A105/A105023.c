/*
 * A105023  a(n) = A102370(n) - n. Or, 2*A103185(n).
 * https://oeis.org/A105023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{ k >= 1 : (n + k) == 0 mod 2^k } 2^k
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; ; ++k)
    {
        /* n + k may overflow a 64‑bit type, so use 128‑bit arithmetic for the test. */
        unsigned __int128 nk   = (unsigned __int128)n + k;
        unsigned __int128 pow2 = (unsigned __int128)1 << k;

        /* If 2^k exceeds n + k, no larger k can satisfy the divisibility. */
        if (pow2 > nk)
            break;

        /* Check if 2^k divides n + k. */
        if ( (nk & (pow2 - 1)) == 0 )
            sum += (seq_t)pow2;
    }
    return sum;
}

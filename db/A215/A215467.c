/*
 * A215467  Length of longest palindromic prefix of (n base 2).
 * https://oeis.org/A215467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the length of the longest palindromic prefix of the binary
   representation of n (OEIS A215467) */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0: binary "0" has a palindromic prefix of length 1 */
    if (n == 0)
        return (seq_t)1;

    /* Find position of the most significant 1 bit (0‑based) */
    idx_t msb = 0;
    idx_t t   = n;
    while ((t >>= 1) != 0)
        msb++;

    idx_t len = msb + 1;          /* total length of binary representation */

    /* Test prefix lengths from longest to shortest */
    for (idx_t L = len; L >= 1; --L) {
        bool ok = true;
        for (idx_t i = 0; i < L / 2; ++i) {
            idx_t bit1 = (n >> (msb - i)) & 1;
            idx_t bit2 = (n >> (msb - (L - 1) + i)) & 1;
            if (bit1 != bit2) {
                ok = false;
                break;
            }
        }
        if (ok)
            return (seq_t)L;      /* first matching L is the answer */
    }

    /* The loop always finds at least L = 1, so this point is unreachable */
    return (seq_t)1;
}

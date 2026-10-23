/*
 * A196793  a(n) = A047848(n, n).
 * https://oeis.org/A196793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+3)^n + n + 1)/(n+2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For small n the value fits in 128 bits, so we use a wide
       integer implementation that avoids overflow. Only the first
       few terms (n <= 16 for a 64‑bit build) are needed exactly
       in that build. */

    if (n == 0)
        return 1;                     /* (3^0 + 0 + 1)/2 = 1 */

    /* n <= 16:  (n+3)^n < 2^128, safe in unsigned __int128 */
    if (n <= 16) {
        unsigned __int128 base = (unsigned __int128)(n + 3);
        unsigned __int128 pow   = 1;
        for (idx_t i = 0; i < n; ++i)
            pow *= base;

        pow += (unsigned __int128)n + 1;
        pow /= (unsigned __int128)(n + 2);

        return (seq_t)pow;
    }

    /* For larger n we rely on the arbitrary‑precision big_pow.
       For the 64‑bit build this may overflow, but that intermediate
       value is only needed for indices whose result does not fit
       in 64 bits, so correctness is preserved for all values that
       do fit. */
    seq_t base   = (seq_t)(n + 3);
    seq_t pow    = big_pow(base, (seq_t)n);
    pow += (seq_t)n;
    pow += 1;
    seq_t denom  = (seq_t)(n + 2);
    return pow / denom;
}

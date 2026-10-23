/*
 * A048328  Numbers that are repdigits in base 3.
 * https://oeis.org/A048328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 0 a(n) = 3^((n+1)/2) - 1 if n is even a(n) = (3^((n+1)/2)
 *            - 1)/2 if n is odd where 3^k is computed with *big* intermediate
 *            to avoid overflow
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 81, u128 n <= 160,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0U)
        return 0;

    /* d = floor((n+1)/2) = n/2 + n%2  */
    idx_t d = (n >> 1) + (n & 1U);

    /* If seq_t is small (<= 64 bits) use 128‑bit intermediate */
    if (sizeof(seq_t) <= sizeof(uint64_t))
    {
        /* Use unsigned __int128 for the power computation */
        #ifdef __SIZEOF_INT128__
            typedef unsigned __int128 uint128_t;
        #else
            /* Fallback: if __int128 is not available, use pow3 for big seq_t */
            return pow3(d) - 1U;   /* should not reach for small seq_t */
        #endif

        uint128_t pow = 1;
        for (idx_t i = 0; i < d; ++i)
            pow *= 3;

        if (n & 1U)            /* odd n */
            return (seq_t)((pow - 1) >> 1);
        else                   /* even n */
            return (seq_t)(pow - 1);
    }

    /* For larger seq_t (e.g. arbitrary‑precision) use pow3 directly */
    seq_t pow = pow3(d);
    if (n & 1U)                /* odd n */
        return (pow - 1) / 2;
    else                       /* even n */
        return pow - 1;
}

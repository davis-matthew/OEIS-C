/*
 * A155014  a(n) = floor(sqrt(n^7)).
 * https://oeis.org/A155014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(n^7))
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 565, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(n^7)) for any idx_t n, returning a value of type seq_t. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;          /* convert index to product type */

    /* compute n^7 using a 128‑bit intermediate (enough for all n
       that need a 64‑bit result).  The loop runs 6 times because
       tmp already contains n. */
    unsigned __int128 tmp = (unsigned __int128)nn;
    for (int i = 0; i < 6; ++i)
        tmp *= (unsigned __int128)nn;

    /* integer square root of tmp: binary search in the 64‑bit range. */
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1) << 64;  /* exclusive upper bound */

    while (low + 1 < high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid <= tmp)
            low = mid;
        else
            high = mid;
    }

    return (seq_t)low;
}

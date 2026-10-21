/*
 * A099026  Array x AND NOT y, read by rising antidiagonals.
 * https://oeis.org/A099026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 9464, u16 n <= 9464, u32 n <= 9464, u64 n <= 9464,
 *            u128 n <= 9464, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A099026: a(n) = (k - t) AND NOT (t),
   where k = floor( (sqrt(8n+1)-1)/2 ) is the antidiagonal index,
   t = n - k(k+1)/2 is the position along that diagonal. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the largest k with k(k+1)/2 <= n */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t k = low;

    unsigned __int128 tri128 = (unsigned __int128)k * (k + 1) / 2;
    idx_t tri     = (idx_t)tri128;          /* k(k+1)/2 fits in idx_t */
    idx_t t       = n - tri;                /* position within diagonal */

    idx_t x = k - t;                        /* coordinate x */
    idx_t y = t;                            /* coordinate y */

    return ((seq_t)x) & ~((seq_t)y);        /* x AND NOT y */
}

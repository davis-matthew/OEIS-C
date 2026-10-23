/*
 * A194481  Number of ways to arrange 4 indistinguishable points on an n X n X
 *          n triangular grid so that no four points are in the same row or
 *          diagonal.
 * https://oeis.org/A194481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*n^8 + 20*n^7 - 30*n^6 - 208*n^5 + 285*n^4 + 140*n^3 -
 *            260*n^2 + 48*n) / 1920 (OEIS A194481, offset 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 33, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use signed __int128 for intermediate calculations to avoid overflow
       for every n for which a(n) fits in seq_t. */
    __int128 t = 5;                 /* 5                          */
    t = t * (__int128)n + 20;       /* 5*n + 20                   */
    t = t * (__int128)n - 30;       /* (5*n + 20)*n - 30          */
    t = t * (__int128)n - 208;      /* ... - 208                  */
    t = t * (__int128)n + 285;      /* ... + 285                  */
    t = t * (__int128)n + 140;      /* ... + 140                  */
    t = t * (__int128)n - 260;      /* ... - 260                  */
    t = t * (__int128)n + 48;       /* ... + 48                   */
    t = t * (__int128)n;            /* multiply by n to get
                                       5*n^8 + 20*n^7 - ... + 48*n
                                     */
    return (seq_t)(t / 1920);       /* final division */
}

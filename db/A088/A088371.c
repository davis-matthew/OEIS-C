/*
 * A088371  Position where n is inserted into the n-th row of triangle
 *          A088370, where the n-th row differs from the prior row only by the
 *          presence of n.
 * https://oeis.org/A088371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(2n‑1) = a(n); a(2n) = n + a(n)
 * Range:     u8 n <= 255, u16 n <= 8993, u32 n <= 8993, u64 n <= 8993,
 *            u128 n <= 8993, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset (sequence starts at n=1). */
    if (n <= 1)
        return (seq_t)n;          /* a(1) = 1; a(0) unused but harmless */

    /* Even index: a(2k) = k + a(k) */
    if ((n & 1) == 0) {
        idx_t k = n >> 1;                     /* k = n/2  (integer division) */
        seq_t ak = a(k);                      /* recursive call for a(k) */
        return (seq_t)k + ak;                 /* sum in seq_t domain */
    }

    /* Odd index: a(2k‑1) = a(k),   k = (n+1)/2 */
    idx_t k = (n + 1) >> 1;
    return a(k);
}

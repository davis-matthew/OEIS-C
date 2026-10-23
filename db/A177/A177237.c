/*
 * A177237  Partial sums of round(n^2/19).
 * https://oeis.org/A177237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 - 11*n + 42)/114)
 * Range:     u8 n <= 24, u16 n <= 154, u32 n <= 895, u64 n <= 895,
 *            u128 n <= 895, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of round(n^2/19), offset 0 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use an unsigned 128‑bit intermediate to avoid overflow */
    unsigned __int128 nn = n;
    unsigned __int128 n2 = nn * nn;      /* n^2 */
    unsigned __int128 n3 = n2 * nn;      /* n^3 */
    unsigned __int128 num = 2*n3 + 3*n2 - 11*nn + 42;

    return (seq_t)(num / 114);
}

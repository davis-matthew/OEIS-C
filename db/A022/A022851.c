/*
 * A022851  a(n) = integer nearest n*x, where x = sqrt(8).
 * https://oeis.org/A022851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to n*sqrt(8)
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t s = nn * nn * 8;          /* s = 8*n^2 */
    seq_t k = floor_sqrt(s);        /* k = floor(sqrt(s)) */
    seq_t r = s - k * k;            /* remainder s - k^2 */
    seq_t cmp = 4 * r;              /* 4*(s - k^2) */
    seq_t bound = 4 * k + 1;        /* 4*k + 1 */

    if (cmp >= bound)
        return k + 1;               /* nearest integer is k+1 */
    else
        return k;                   /* nearest integer is k */
}

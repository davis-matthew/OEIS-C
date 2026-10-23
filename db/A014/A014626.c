/*
 * A014626  Number of intersection points of diagonals of an n-gon in general
 *          position, plus number of vertices.
 * https://oeis.org/A014626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,1) + C(n,4) = n + n*(n-1)*(n-2)*(n-3)/24
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 260, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n;                 /* C(n,1) = n */
    if (n >= 4) {
        seq_t prod = (seq_t)n * (seq_t)(n - 1);
        prod *= (seq_t)(n - 2);
        prod *= (seq_t)(n - 3);
        prod /= 24;                       /* exact division by 4! = 24 */
        res += prod;                      /* add C(n,4) */
    }
    return res;
}

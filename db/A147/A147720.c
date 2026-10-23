/*
 * A147720  Riordan array (1, x(1-x)/(1-3x)).
 * https://oeis.org/A147720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 67, u32 n <= 212, u64 n <= 707,
 *            u128 n <= 2566, big any n
 * Helpers:   binom, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A147720 – flattened entries of the Riordan array (1, x(1-x)/(1-3x))
    a(n) = T(r,c)
    where  r = floor((sqrt(8n+1)-1)/2)   (row index)
            c = n - r(r+1)/2            (column index)
            m = r - c

    T(m,k) = sum_{i=0}^{min(k,m)} (-1)^i *
              binom(k,i) *
              binom(k + m - i - 1, m - i) *
              3^(m - i)
    (binom(i,j) = 0 if j>i).  For k>0 this formula gives the required
    integer values; for k=0 it yields 0 unless m=0 (the first entry).
*/

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/binom.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2  */
    idx_t r = (idx_t)((sqrt((long double)8.0L * n + 1.0L) - 1.0L) / 2.0L);
    /* adjust in case of rounding errors */
    while (((__int128)r * (r + 1) / 2) > n) --r;
    while (((__int128)(r + 1) * (r + 2) / 2) <= n) ++r;

    idx_t offset = n - r * (r + 1) / 2;   /* column index */
    idx_t c = offset;
    if (c == 0)                         /* first column */
        return (r == 0) ? (seq_t)1 : (seq_t)0;

    idx_t m = r - c;                    /* n - k */

    seq_t pos_sum = 0;
    seq_t odd_sum = 0;
    idx_t max_i = (c < m) ? c : m;
    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t part1 = binom(c, i);
        /* binom(c + m - i - 1, m - i) */
        idx_t top   = c + m - i - 1;
        idx_t bottom = m - i;
        seq_t part2 = (bottom <= top) ? binom(top, bottom) : (seq_t)0;
        seq_t part3 = pow3(bottom);
        seq_t term  = part1 * part2 * part3;
        if ((i & 1) == 0)
            pos_sum += term;
        else
            odd_sum += term;
    }
    /* pos_sum >= odd_sum for all inputs */
    return pos_sum - odd_sum;
}

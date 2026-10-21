/*
 * A086270  Rectangular array T(k,n) of polygonal numbers, by antidiagonals.
 * https://oeis.org/A086270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* AOI: Rectangular array T(k,n) of polygonal numbers by antidiagonals.
   Formula:  a(n) = T(i,j) where (i,j) is the nth cell visited when
             the cells are ordered by increasing i+j (antidiagonals)
             and, for each antidiagonal, by decreasing j.
          T(i,j) = i*binomial(j,2) + j
          binomial(j,2) = j*(j-1)/2.
*/
#include <stdint.h>

static inline seq_t binom2(idx_t k)
{
    seq_t kk = (seq_t)k;
    return kk * (kk - 1) / 2;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t s = 2; ; ++s) {            /* s = i + j  (starting from 2) */
        for (idx_t j = s - 1; j >= 1; --j) {
            idx_t i = s - j;
            seq_t val = (seq_t)i * binom2(j) + (seq_t)j;
            ++count;
            if (count == n)
                return val;
        }
    }
}

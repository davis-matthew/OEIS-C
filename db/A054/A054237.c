/*
 * A054237  Pair addition table cast as diagonal numbers.
 * https://oeis.org/A054237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   Let P(k) = (i,j) be the pair obtained by casting the index k
 *            into diagonal order: k = T_{i+j} + j with T_s = s*(s+1)/2. Then
 *            a(n) is the index of the pair ( P(i).first + P(j).first ,
 *            P(i).second + P(j).second ). Equivalently: let (i,j) = P(n);
 *            (i1,j1) = P(i); (i2,j2) = P(j); a(n) = T_{i1+i2 + j1+...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   antidiag
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/antidiag.h"

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiag(n, &i, &j);

    idx_t i1, j1;
    antidiag(i, &i1, &j1);

    idx_t i2, j2;
    antidiag(j, &i2, &j2);

    idx_t i_sum = i1 + i2;
    idx_t j_sum = j1 + j2;
    idx_t s2    = i_sum + j_sum;

    seq_t tri = (seq_t)((unsigned __int128)s2 * (s2 + 1) / 2);
    return tri + (seq_t)j_sum;
}

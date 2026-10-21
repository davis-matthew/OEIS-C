/*
 * A218828  Reluctant sequence of reverse reluctant sequence A004736.
 * https://oeis.org/A218828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (t1^2 + 3*t1 + 4)/2 - n1, where t = floor[(-1 + sqrt(8*n
 *            - 7))/2] (largest k with k(k+1)/2 < n) n1 = n - t(t+1)/2 t1 =
 *            floor[(-1 + sqrt(8*n1 - 7))/2] (largest k with k(k+1)/2 < n1)
 * Range:     u8 n <= 8509, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   _triangular_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/_triangular_index.h"

seq_t a(idx_t n)
{
    idx_t t  = _triangular_index(n);
    seq_t t_seq  = (seq_t)t;
    seq_t n1_seq = (seq_t)n - (t_seq * (t_seq + 1) / 2);
    idx_t n1 = (idx_t)n1_seq;

    idx_t t1 = _triangular_index(n1);
    seq_t t1_seq = (seq_t)t1;

    seq_t part = t1_seq * t1_seq + (seq_t)3 * t1_seq + (seq_t)4;
    part = part / 2;
    return part - n1_seq;
}

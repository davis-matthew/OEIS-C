/*
 * A195679  Order of n-th homotopy group of the topological group O(oo), with
 *          -1 if the homotopy group is Z.
 * https://oeis.org/A195679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-8) for n > 7; a(0)=2, a(1)=2, a(2)=1, a(3)=-1,
 *            a(4)=1, a(5)=1, a(6)=1, a(7)=-1
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t table[8] = {2, 2, 1, -1, 1, 1, 1, -1};
    return table[(size_t)(n & 7ULL)];
}

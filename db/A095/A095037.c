/*
 * A095037  An example of a (v,k,lambda)=(23,11,5) cyclic difference set.
 * https://oeis.org/A095037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = the nth element of the unique 11‑element * cyclic
 *            difference set of Z/23 with parameters (v,k,λ)=(23,11,5), *
 *            using offset 1. For n>11, a(n)=0
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t diffset[11] = {
    1, 2, 3, 4, 6, 8, 9, 12, 13, 16, 18
};

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n > (idx_t)11) return 0;
    /* n>=1 and n<=11 */
    return diffset[n - 1];
}

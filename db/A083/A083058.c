/*
 * A083058  Number of eigenvalues equal to 1 of n X n matrix A(i,j)=1 if j=1
 *          or i divides j.
 * https://oeis.org/A083058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n - 1 - ⌊log₂(n)⌋ (for n > 1; a(1)=1)
 * Range:     u8 n <= 264, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   _floor_log2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This result follows from the identity a(n)=n-A070939(n) and the known formula for A070939. */

#include <stdint.h>
#include "../utils/_floor_log2_u64.h"

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)1;
    }
    uint64_t log2n = _floor_log2_u64((uint64_t)n);
    return (seq_t)n - (seq_t)1 - (seq_t)log2n;
}

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
 * Validated: standard, pass, 89 terms through n = 7507 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This result follows from the identity a(n)=n-A070939(n) and the known formula for A070939. */

#include <stdint.h>

/* Compute ⌊log₂(x)⌋ for x≥1.  */
static inline uint64_t _floor_log2_u64(uint64_t x) {
    uint64_t r = 0;
    if (x >> 32) { r += 32; x >>= 32; }
    if (x >> 16) { r += 16; x >>= 16; }
    if (x >>  8) { r += 8;  x >>= 8;  }
    if (x >>  4) { r += 4;  x >>= 4;  }
    if (x >>  2) { r += 2;  x >>= 2;  }
    if (x >>  1) { r += 1; }
    return r;
}

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)1;
    }
    uint64_t log2n = _floor_log2_u64((uint64_t)n);
    return (seq_t)n - (seq_t)1 - (seq_t)log2n;
}

/*
 * A240908  The sequency numbers of the 8 rows of a version of the
 *          Hadamard-Walsh matrix of order 8.
 * https://oeis.org/A240908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = permutation of row indices in sequency order for the
 *            Hadamard–Walsh matrix of order 2^k, where k = max(3 ,
 *            floor(log2(n-1))+1). The function works for arbitrary n ≥ 1 and
 *            is valid for all builds supporting uint64_t for idx_t and an
 *            unsigned integer type seq_t
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n < 1, return 0 */

    idx_t idx = n - 1;                   /* 0‑based index */

    /* Determine number of bits needed for the current block.
       For the first 8 indices we need 3 bits; for larger indices
       the width grows with the magnitude of idx. */
    unsigned width = 3;                  /* at least 3 bits (order 8) */
    if (idx > 0) {
        unsigned w = 0;
        idx_t tmp = idx;
        while (tmp) { ++w; tmp >>= 1; }
        if (w > width) width = w;
    }

    /* Bit‑reverse idx using the determined width. */
    seq_t r = 0;
    for (unsigned i = 0; i < width; ++i) {
        if (idx & ((idx_t)1 << i))
            r |= ((seq_t)1 << (width - 1 - i));
    }

    /* Compute the permutation value:
       j = r XOR (r >> 1) XOR (r >> 2) ... XOR (r >> (width-1)) */
    seq_t j = r;
    for (unsigned shift = 1; shift < width; ++shift) {
        j ^= (r >> shift);
    }

    return j;
}

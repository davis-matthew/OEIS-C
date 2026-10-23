/*
 * A175913  Convert to decimal the number resulting from performing binary
 *          xnor on the corresponding digits in the left and right binary
 *          concatenations of the integers one to n.
 * https://oeis.org/A175913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the binary number obtained by performing bitwise XNOR on
 *            the left concatenation and the right concatenation of the binary
 *            representations of the integers 1 to n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bitlen_seq_t.h"

/* Return the bit length of a 64‑bit unsigned integer */
static inline int bitlen_u64(idx_t x) {
    int l = 0;
    while (x) {
        ++l;
        x >>= 1;
    }
    /* x is never 0 for this use‑case, but return 0 for completeness */
    return l;
}

seq_t a(idx_t n) {
    seq_t left = 0, right = 0;

    /* Build the left concatenation (1,2,...,n) */
    for (idx_t i = 1; i <= n; ++i) {
        int len = bitlen_u64(i);
        left = (left << len) | (seq_t)i;
    }

    /* Build the right concatenation (n,n-1,...,1) */
    for (idx_t i = n; i >= 1; --i) {
        int len = bitlen_u64(i);
        right = (right << len) | (seq_t)i;
    }

    seq_t x = left ^ right;                 /* XOR part */
    unsigned int len_bits = bitlen_seq_t(left); /* bit length of the strings */

    /* Construct a mask with len_bits ones */
    seq_t mask;
    if (len_bits == 0) {
        mask = 0;
    } else if (sizeof(seq_t) <= 8 && len_bits == sizeof(seq_t)*8) {
        /* Shifting by the width of the type would be UB; use all‑ones mask */
        mask = ~( (seq_t)0 );
    } else {
        mask = (((seq_t)1) << len_bits) - 1;
    }

    /* XNOR = complement of XOR masked to the proper length */
    seq_t result = (~x) & mask;
    return result;
}

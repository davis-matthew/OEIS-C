/*
 * A103215  Numbers congruent to {1, 2, 5, 10, 13, 17} mod 24.
 * https://oeis.org/A103215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*floor((n-1)/6) + {1,2,5,10,13,17}[ (n-1)%6 ]
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t, required by some environments */

seq_t a(idx_t n) {
    /* a(1) is defined; for n==0 the behavior is irrelevant */
    if (n == 0) return (seq_t)0;

    /* residues of the sequence modulo 24 in the order of appearance */
    static const seq_t residues[6] = {
        (seq_t)1, (seq_t)2, (seq_t)5,
        (seq_t)10, (seq_t)13, (seq_t)17
    };

    idx_t q = (n - 1) / 6;      /* complete blocks of six numbers */
    idx_t r = (n - 1) % 6;      /* position within the current block */

    /* compute 24*q using seq_t to avoid intermediate overflow in the
       index type */
    return residues[r] + (seq_t)24 * (seq_t)q;
}

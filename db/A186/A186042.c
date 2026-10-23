/*
 * A186042  Numbers of the form 2*k + 1, 3*k + 2, or 5*k + 3.
 * https://oeis.org/A186042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30 * floor((n-1)/22) + r[(n-1) mod 22]
 * Range:     u8 n <= 187, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where r = {1,2,3,5,7,8,9,11,13,14,15,17,18,19,20,21,23,25,26,27,28,29} */
/* Numbers of the form 2*k+1, 3*k+2, or 5*k+3.  Period 22. */

#include <stddef.h>

static const seq_t residues[22] = {
    (seq_t)1,  (seq_t)2,  (seq_t)3,  (seq_t)5,  (seq_t)7,
    (seq_t)8,  (seq_t)9,  (seq_t)11, (seq_t)13, (seq_t)14,
    (seq_t)15, (seq_t)17, (seq_t)18, (seq_t)19, (seq_t)20,
    (seq_t)21, (seq_t)23, (seq_t)25, (seq_t)26, (seq_t)27,
    (seq_t)28, (seq_t)29
};

seq_t a(idx_t n)
{
    idx_t off = (n - 1) % 22;
    idx_t blk = (n - 1) / 22;
    seq_t block_val = (seq_t)30 * (seq_t)blk;
    return block_val + residues[off];
}

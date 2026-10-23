/*
 * A122570  Consider the Thue-Morse sequence (A010060) at each iteration. Read
 *          each reversed string as a binary number and convert it to a
 *          decimal number.
 * https://oeis.org/A122570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) is the value of the reversed Thue–Morse string of length
 *            2^(n‑1), interpreted as a binary number. The sequence satisfies
 *            a(1) = 0 and for n ≥ 2: a(n) = (( (2^(2^(n‑2)) – 1) – a(n‑1) )
 *            << 2^(n‑2)) | a(n‑1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* the sequence is defined for n ≥ 1, but guard anyway */
        return (seq_t)0;

    seq_t block = 0; /* value of a(k) for current k */
    idx_t  len   = 1;/* current block length in bits (2^(k-1)) */

    /* iterate to build a(n) from a(1) = 0 */
    for (idx_t k = 1; k < n; ++k) {
        /* mask gives a number with all lower 'len' bits set to 1 */
        seq_t mask = ((seq_t)1 << len) - 1;
        /* complement of previous block within 'len' bits */
        seq_t comp = mask - block;
        /* form new block: complement block followed by old block */
        block = (comp << len) | block;
        /* next block has double the length */
        len <<= 1;
    }
    return block;
}

/*
 * A181753  Universal sequence of period 56 which contains every 3-subset of
 *          {1,2,...,8} exactly once.
 * https://oeis.org/A181753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((B[(n-1)%7] - 1 + 5*((n-1)/7)%8) % 8) + 1 where B =
 *            [1,3,5,6,7,2,5] is the first 7 terms of the sequence. The
 *            sequence repeats every 56 terms, because each block of length 7
 *            is obtained from the previous one by adding 5 modulo 8
 * Range:     u8 n <= 168, u16 n <= 168, u32 n <= 168, u64 n <= 168,
 *            u128 n <= 168, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute the position inside the 7‑term block and the block index. */
    idx_t pos   = (n - 1) % 7;       /* 0 … 6 */
    idx_t block = ((n - 1) / 7) % 8; /* 0 … 7 */

    /* Base block contains the first 7 terms of the sequence. */
    static const seq_t B[7] = {1, 3, 5, 6, 7, 2, 5};

    /* Shift amount is 5 * block modulo 8. */
    seq_t shift = (seq_t)((5 * block) % 8);

    /* Compute the term: shift the base value, wrap modulo 8, convert back to 1‑8. */
    seq_t val = (seq_t)B[pos];
    seq_t res = (((val - 1u) + shift) % 8u) + 1u;
    return res;
}

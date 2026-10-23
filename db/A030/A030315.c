/*
 * A030315  Write n in base 2, complement each digit (d -> 1-d) and append the
 *          complemented digits to the sequence.
 * https://oeis.org/A030315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A030315: write n in base 2, complement each digit, and append.
   a(n) equals the (n‑th) bit of the concatenated complements.           */
/* Complexity: O(n) in the worst case, < 2 seconds for n ≈ 10000.           */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t idx = n;      /* remaining position within the current block   */
    idx_t m = 0;        /* current number being processed               */

    while (1) {
        /* length of the binary representation of m   */
        idx_t len;
        if (m == 0) {
            len = 1;
        } else {
            /* number of bits = floor(log2(m)) + 1 */
            len = 64 - __builtin_clzll((unsigned long long)m);
        }

        if (idx < len) {   /* the desired bit lies in the current block */
            /* position of the bit in the binary representation
               counting from the most significant bit (MSB) */
            idx_t bit_pos = len - 1 - idx;
            uint8_t bit = ((unsigned long long)m >> bit_pos) & 1ULL;
            return (seq_t)(1 - bit);   /* complement of the bit */
        }

        /* move to the next block */
        idx -= len;
        ++m;
    }
}

/*
 * A122953  a(n) = number of distinct positive integers represented in binary
 *          which are substrings of binary expansion of n.
 * https://oeis.org/A122953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct positive integers that appear * as
 *            contiguous binary substrings of the binary representation of n.
 *            * * The algorithm enumerates all substrings of the binary string
 *            of n, * keeps a small array of the seen values (at most 64*65/2
 *            distinct * substrings), and counts the dist...
 * Range:     u8 n <= 286, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* seq_t and idx_t are defined by the build.  idx_t is an unsigned 64‑bit
 * integer (uint64_t).  seq_t may be an arbitrary‑precision integer type. */
seq_t a(idx_t n)
{
    /* Undefined for n == 0: return 0. */
    if (n == 0) return (seq_t)0;

    /* Determine the length of the binary representation of n. */
    unsigned bitlen = 0;
    idx_t tmp = n;
    while (tmp) { ++bitlen; tmp >>= 1; }

    /* Maximum number of substrings for 64 bits is 64*65/2 = 2080. */
    static seq_t seen[2080];
    unsigned seen_len = 0;
    unsigned distinct = 0;

    for (unsigned i = 0; i < bitlen; ++i) {
        seq_t val = (seq_t)0;
        for (unsigned j = i; j < bitlen; ++j) {
            /* Extract the bit at position j (counting from the left). */
            unsigned bit = (n >> (bitlen - 1 - j)) & 1U;
            val = (val << 1) | (seq_t)bit;

            if (val == (seq_t)0) continue;          /* skip zero values   */
            /* Linear search in the small seen array. */
            bool found = false;
            for (unsigned k = 0; k < seen_len; ++k) {
                if (seen[k] == val) { found = true; break; }
            }
            if (!found) {
                seen[seen_len++] = val;
                ++distinct;
            }
        }
    }
    return (seq_t)distinct;
}

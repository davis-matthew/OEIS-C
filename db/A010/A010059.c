/*
 * A010059  Another version of the Thue-Morse sequence: let A_k denote the
 *          first 2^k terms; then A_0 = 1 and for k >= 0, A_{k+1} = A_k B_k,
 *          where B_k is obtained from A_k by interchanging 0's and 1's.
 * https://oeis.org/A010059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 1 - (popcount(n) mod 2) // complement of Thue‑Morse
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute the parity of the number of 1‑bits in n (popcount(n) mod 2). */
    idx_t x = n;
    /* Parallel XOR method, works for 64‑bit n. */
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    unsigned int parity = (unsigned int)(x & 1U);

    /* Return the complement (0 → 1, 1 → 0) as a seq_t. */
    return parity ? (seq_t)0 : (seq_t)1;
}

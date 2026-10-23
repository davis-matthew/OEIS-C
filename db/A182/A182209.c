/*
 * A182209  a(n) is the least m >= n, such that the Hamming distance D(n,m) =
 *          3.
 * https://oeis.org/A182209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the least m ≥ n such that the Hamming distance between n and m is 3
   (i.e., popcount(n XOR m) == 3).  This implements the definition of sequence
   A182209.  The algorithm searches upward from n until a number with exactly
   three differing bits is found.  For 64‑bit indices the search operates on an
   unsigned __int128 value so that very large m (e.g., just beyond 2^64‑1)
   can be represented.  */
#include <stdint.h>

/* Count set bits in a 128‑bit value using two 64‑bit popcounts. */
static int popcount128(unsigned __int128 x)
{
    return __builtin_popcountll((uint64_t)x) +
           __builtin_popcountll((uint64_t)(x >> 64));
}

/* Return a(n) for the OEIS sequence A182209. */
seq_t a(idx_t n)
{
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 m128 = n128;

    while (1) {
        if (popcount128(n128 ^ m128) == 3)
            return (seq_t)m128;
        ++m128;
    }
}

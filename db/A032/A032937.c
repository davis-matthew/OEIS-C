/*
 * A032937  Numbers k whose base-2 representation Sum_{i=0..m} d(i)*2^(m-i)
 *          has d(i)=0 for all odd i, excluding 0. Here m is the position of
 *          the leading bit of k.
 * https://oeis.org/A032937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A032937 – Numbers whose binary representation has zeros in all odd positions
   (counting from the most significant bit).  
   Algorithm: find the length L of the binary representation that contains the n-th
   number, compute the offset of that number among all numbers of length L,
   then rebuild the value by inserting the required zero bits. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    // offset is 1; n==0 is unused

    /* Find the binary length L that covers the n-th number. */
    idx_t cumulative = 0;
    idx_t L = 0;
    while (1) {
        ++L;
        idx_t evens  = (L + 1) / 2;          // number of even indices in a length‑L string
        seq_t count  = pow2(evens - 1);     // how many numbers have this length
        cumulative += (idx_t)count;        // cumulative count up to this length
        if (cumulative >= n) break;
    }

    idx_t evens     = (L + 1) / 2;
    idx_t free_bits = evens - 1;             // bits that can vary (excluding the leading 1)

    /* Offset of the desired number within the group of length L (0‑based). */
    idx_t prev_cum   = cumulative - (idx_t)pow2(free_bits);
    idx_t offset     = n - prev_cum - 1;     // 0‑based index among numbers of length L

    /* Special case for the two shortest lengths: L = 1 or 2. */
    if (L <= 2) {
        return pow2(L - 1);                  // 1 → 1, 2 → 2
    }

    /* Reconstruct the number: start with the leading 1 at position L‑1. */
    seq_t result = pow2(L - 1);

    /* Position of the next free bit (after the first zero that follows the leading 1). */
    idx_t pos = L - 3;                       // j = L-3, L-5, …

    /* Insert the remaining free bits (from most significant to least). */
    for (idx_t k = 1; k <= free_bits; ++k) {
        if ((offset >> (free_bits - k)) & 1U) {
            result |= pow2(pos);
        }
        pos -= 2;
    }

    return result;
}

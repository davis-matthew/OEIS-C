/*
 * A175245  Let S be the set of distinct positive integers whose binary
 *          representations are substrings in the binary representation of n.
 *          a(n) = the number of elements of S that are coprime to all other
 *          elements of S.
 * https://oeis.org/A175245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of elements of S that are coprime to all other
 *            elements of S, where S is the set of distinct positive integers
 *            whose binary representations are substrings of the binary
 *            representation of n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

#define MAX_SUBSTRINGS (64*65/2)

seq_t a(idx_t n)
{
    /* Compute the number of bits in n. */
    uint64_t temp = (uint64_t)n;
    int len = 0;
    while (temp) { len++; temp >>= 1; }

    /* Array to hold distinct substring values.  Max 2080 for a 64‑bit number. */
    seq_t S[MAX_SUBSTRINGS];
    size_t count = 0;

    /* Enumerate all contiguous substrings of the binary string of n. */
    for (int start = 0; start < len; ++start) {
        seq_t val = 0;
        for (int end = start; end < len; ++end) {
            int bitPos = len - 1 - end;                 /* MSB has position 0 */
            int bit = ((uint64_t)n >> bitPos) & 1ULL;
            val = (val << 1) | (seq_t)bit;
            if (val == 0) continue;                     /* skip all‑zero substrings */

            /* Insert distinct values only. */
            bool present = false;
            for (size_t k = 0; k < count; ++k) {
                if (S[k] == val) { present = true; break; }
            }
            if (!present) {
                S[count++] = val;
            }
        }
    }

    /* Count elements coprime to every other element in S. */
    seq_t result = 0;
    for (size_t i = 0; i < count; ++i) {
        bool coprime = true;
        for (size_t j = 0; j < count; ++j) {
            if (i == j) continue;
            if (gcd(S[i], S[j]) != 1) { coprime = false; break; }
        }
        if (coprime) result++;
    }

    return result;
}

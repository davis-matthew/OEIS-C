/*
 * A138663  a(n) = number of positive integers k, k <= n, where the number of
 *          ones in the binary representation of each k is coprime to n.
 * https://oeis.org/A138663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = # { k | 1 ≤ k ≤ n , gcd(popcount(k), n) = 1 }
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    uint64_t limit = n;                     /* n fits in uint64_t (max 10000) */
    for (uint64_t k = 1; k <= limit; ++k) {
        uint64_t pc = __builtin_popcountll(k);       /* number of 1s in binary repr. */
        seq_t g = gcd((seq_t)pc, (seq_t)limit);
        if (g == (seq_t)1) {
            ++result;
        }
    }
    return result;
}

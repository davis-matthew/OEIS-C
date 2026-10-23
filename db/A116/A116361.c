/*
 * A116361  Smallest k such that n XOR n*2^k = n*(2^k + 1).
 * https://oeis.org/A116361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest k ≥ 0 such that n XOR (n << k) == n*(2^k + 1)
 *            Equivalent to find minimal k with no two set bits of n separated
 *            by k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    bool diff[65] = {0};          /* diff[d] true if some pair of set bits differ by d */

    for (int i = 0; i < 64; ++i) {
        if (n & ((idx_t)1 << i)) {
            for (int j = i + 1; j < 64; ++j) {
                if (n & ((idx_t)1 << j))
                    diff[j - i] = true;
            }
        }
    }

    for (int k = 1; k <= 64; ++k)
        if (!diff[k])
            return (seq_t)k;

    /* unreachable for all n, but return safe value */
    return (seq_t)64;
}

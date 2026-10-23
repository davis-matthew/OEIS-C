/*
 * A171691  Number of unordered partitions {k1, k2} of n such that k1 and k2
 *          are nonnegative nonprimes A141468.
 * https://oeis.org/A171691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered partitions {k1, k2} of n such that *
 *            both k1 and k2 are non‑prime (including 0 and 1). Index n ≥ 1, *
 *            offset = 1
 * Range:     u8 n <= 689, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1; guard for completeness */
    seq_t cnt = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        if (!is_prime(k) && !is_prime(n - k))
            ++cnt;
    }
    return cnt;
}

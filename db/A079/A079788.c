/*
 * A079788  a(n) = count of numbers <= n for which the number of divisors is
 *          also <= tau(n).
 * https://oeis.org/A079788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{k | 1 ≤ k ≤ n and τ(k) ≤ τ(n)}| (τ = divisor count)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence is defined for n ≥ 1 */

    uint64_t tau_n = divisor_count_u64((uint64_t)n);
    seq_t cnt = 0;

    for (idx_t k = 1; k <= n; ++k)
        if (divisor_count_u64((uint64_t)k) <= tau_n)
            ++cnt;

    return cnt;
}

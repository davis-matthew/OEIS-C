/*
 * A186410  Number of "ON" cells at n-th stage of three-dimensional version of
 *          the cellular automaton A183060 using cubes.
 * https://oeis.org/A186410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + (4/5) * Σ_{i=1}^{n-1} 5^{popcount(i)} where
 *            popcount(i) is the number of 1‑bits in the binary representation
 *            of i. Implemented with O(n) time and O(1) extra space (apart
 *            from a static table of powers of 5)
 * Range:     u8 n <= 11, u16 n <= 95, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;

    /* Precompute powers 5^k up to k = 64 (max popcount for idx_t = uint64_t) */
    static bool pow5_init = false;
    static seq_t pow5[65];
    if (!pow5_init) {
        pow5[0] = (seq_t)1;
        for (idx_t i = 1; i <= 64u; ++i)
            pow5[i] = pow5[i - 1] * (seq_t)5;
        pow5_init = true;
    }

    seq_t sum_div5 = (seq_t)0;
    /* Σ 5^{popcount(i)-1} over i = 1..n-1  (since each term is divisible by 5) */
    for (idx_t i = 1u; i < n; ++i) {
        int pc = __builtin_popcountll(i);   /* Popcount of i (>=1 for i>=1) */
        sum_div5 += pow5[pc - 1u];
    }

    /* Result = n + 4 * sum_div5 */
    seq_t result = (seq_t)n + sum_div5 * (seq_t)4;
    return result;
}

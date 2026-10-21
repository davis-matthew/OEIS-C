/*
 * A070941  Length of binary representation of 2n+1.
 * https://oeis.org/A070941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of binary representation of 2n+1
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 2*n + 1 in 64‑bit arithmetic (n ≤ 10000 for tests). */
    uint64_t x = ((uint64_t)n << 1) + 1;   /* x ≥ 1 */

    /* floor(log2(x)) = 63 - __builtin_clzll(x) for x > 0 */
    uint64_t flog2 = 63 - __builtin_clzll(x);

    /* Binary length = floor(log2(x)) + 1 */
    return (seq_t)(flog2 + 1);
}

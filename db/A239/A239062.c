/*
 * A239062  Number of integers x, 1 <= x <= n, such that x^x == 0 (mod n).
 * https://oeis.org/A239062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers x with 1 ≤ x ≤ n such that x^x ≡ 0
 *            (mod n)
 * Range:     u8 n <= 315, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t mod = (seq_t)n;          /* cast index to seq_t */
    for (idx_t x = 1; x <= n; ++x)
    {
        if (mod_pow((seq_t)x, x, mod) == 0)
            ++count;
    }
    return count;
}

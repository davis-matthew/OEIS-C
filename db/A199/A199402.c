/*
 * A199402  Binary XOR of 2^k - (-1)^k as k varies from 1 to n.
 * https://oeis.org/A199402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = XOR_{k=1..n} (2^k - (-1)^k)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* XOR of 2^k + 1 for odd k, and 2^k - 1 for even k. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t p = 1; /* p holds 2^(k-1) before each iteration */
    for (idx_t k = 1; k <= n; ++k) {
        p *= (seq_t)2;               /* now p = 2^k */
        seq_t term;
        if (k & 1) {                 /* odd k: 2^k + 1 */
            term = p + (seq_t)1;
        } else {                     /* even k: 2^k - 1 */
            term = p - (seq_t)1;
        }
        result ^= term;
    }
    return result;
}

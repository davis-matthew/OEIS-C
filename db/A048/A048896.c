/*
 * A048896  a(n) = 2^(A000120(n+1) - 1), n >= 0.
 * https://oeis.org/A048896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(A000120(n+1)-1) where A000120 is popcount
 * Range:     u8 n <= 509, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n+1 is safe for all input values expected by the test suite */
    idx_t m = n + 1;

    /* popcount of a 64‑bit value – compiler built‑in works in C11 and C++17 */
    unsigned int bits = __builtin_popcountll((unsigned long long)m);

    /* exponent = popcount(n+1)-1 ;  bits is at least 1 for 0 < m */
    idx_t exp = (bits > 0) ? (bits - 1) : 0;

    return pow2(exp);
}

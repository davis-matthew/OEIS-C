/*
 * A049802  a(n) = n mod 2 + n mod 4 + ... + n mod 2^k, where 2^k <= n <
 *          2^(k+1).
 * https://oeis.org/A049802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊log₂(n)⌋} (n mod 2^i) for n ≥ 1
 * Range:     u8 n <= 347, u16 n <= 9426, u32 n <= 9426, u64 n <= 9426,
 *            u128 n <= 9426, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is at least 1. Compute k such that 2^k ≤ n < 2^(k+1). */
    unsigned int k = 0;
    idx_t t = n;
    while (t >>= 1)
        k++;

    seq_t sum = 0;
    /* Sum n mod 2^i for i = 1 .. k.  For power‑of‑two modulus, use bit masking. */
    for (unsigned int i = 1; i <= k; ++i)
    {
        idx_t mask = ((idx_t)1 << i) - 1;
        sum += (seq_t)(n & mask);   /* n & mask == n mod 2^i */
    }
    return sum;
}

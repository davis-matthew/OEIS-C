/*
 * A199397  Binary XOR of 3^k as k varies from 0 to n.
 * https://oeis.org/A199397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = XOR_{k=0}^{n} 3^k
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 254 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    seq_t result = 0;          /* accumulator for XOR */
    seq_t term   = 1;          /* current power 3^k */

    for (idx_t k = 0; k <= n; ++k)
    {
        result ^= term;        /* include current power */

        /* prepare next power only if we have not reached the last one */
        if (k != n)
            term = mul_small(term, 3); /* term *= 3 */
    }

    return result;
}

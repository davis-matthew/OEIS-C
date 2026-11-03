/*
 * A156892  Denominator of Euler(n, 1/23).
 * https://oeis.org/A156892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 23^n * 2^{v2(n+1)} where v2(k) is the exponent of 2
 *            dividing k
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 200 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    seq_t val = 1;                     /* start with 23^0 = 1  */
    for (idx_t i = 0; i < n; ++i)      /* compute 23^n */
        val = mul_small(val, (unsigned int)23);

    unsigned int shift = (unsigned int)__builtin_ctzll((uint64_t)n + 1U); /* v2(n+1) */
    for (unsigned int i = 0; i < shift; ++i)                             /* * 2^shift */
        val = mul_small(val, 2);

    return val;
}

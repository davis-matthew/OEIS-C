/*
 * A198970  a(n) = 4*10^n - 1.
 * https://oeis.org/A198970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*10^n - 1 (recurrence: a(0)=3, a(n)=10*a(n-1)+9 for n>0)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 199 (2026-11-01)
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

/* Compute term a(n) of OEIS A198970 */
seq_t a(idx_t n)
{
    /* Start with a(0) = 3 */
    seq_t value = (seq_t)3;

    /* Apply the recurrence a(i)=10*a(i-1)+9 for i=1..n */
    for (idx_t i = 1; i <= n; ++i)
    {
        value = mul_small(value, 10);  /* value *= 10 */
        value += (seq_t)9;             /* value += 9   */
    }

    return value;
}

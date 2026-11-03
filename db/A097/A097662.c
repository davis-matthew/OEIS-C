/*
 * A097662  a(n) = A002720(n) - 1.
 * https://oeis.org/A097662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002720(n) – 1 a(n) = Sum_{k=0..n} n!^2 / (k! (n-k)!^2) –
 *            1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 439 (2026-11-01)
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
    if (n == 0) return 0;

    seq_t sum  = 0;
    seq_t term = 1;          /* term for k = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        sum += term;

        if (k == n) break;

        idx_t d = n - k;                 /* (n-k) */
        /* term = term * (n-k)^2 / (k+1) */
        term = mul_small(term, (unsigned int)(d * d));
        term = term / (k + 1);
    }

    return sum - 1;     /* subtract the initial +1 in the formula */
}

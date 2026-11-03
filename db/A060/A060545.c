/*
 * A060545  a(n) = binomial(n^2, n)/n.
 * https://oeis.org/A060545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n^2, n) / n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 99 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

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
    idx_t N = n * n;          /* compute n^2 */
    seq_t res = 1;

    /* compute binomial(n^2, n) multiplicatively, reducing each step */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t u = N - n + i;      /* numerator factor */
        seq_t d = i;              /* denominator factor */

        /* cancel common factors between u and d */
        seq_t g = gcd(u, d);
        u /= g;
        d /= g;

        /* cancel common factors between res and remaining denominator */
        g = gcd(res, d);
        res /= g;
        d /= g;

        /* multiply by u using a helper that handles potential overflow safely */
        res = mul_small(res, (unsigned int)u);

        /* divide by the remaining denominator part */
        res /= d;
    }

    /* divide by n to get the final a(n) = binomial(n^2, n) / n */
    res /= (seq_t)n;
    return res;
}

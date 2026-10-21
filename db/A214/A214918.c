/*
 * A214918  a(n) is the least m > 0 such that Fibonacci(n-m) divides
 *          Fibonacci(n+2+m).
 * https://oeis.org/A214918
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - k, where k is the largest divisor of (2n+2) that is
 *            strictly less than n. This follows from the fact that
 *            Fibonacci(k) | Fibonacci(m) ⇔ k | m. Therefore the divisibility
 *            condition in the definition reduces to finding the largest
 *            divisor k < n of 2n+2; the required m is then n-k
 * Range:     u8 n <= 261, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2) {
        /* behaviour below the offset is irrelevant */
        return 0;
    }

    /* use 64‑bit arithmetic for the intermediate calculation;
       n is assumed ≤ 10000 in the problem limits. */
    uint64_t nn = (uint64_t)n;
    uint64_t t  = 2 * nn + 2;           /* 2n+2 */

    uint64_t best = 1;                  /* 1 always divides t */

    /* enumerate all divisors of t by trial division up to sqrt(t) */
    for (uint64_t d = 1; d * d <= t; ++d) {
        if (t % d == 0) {
            uint64_t d1 = d;
            uint64_t d2 = t / d;

            if (d1 < nn && d1 > best) best = d1;
            if (d2 < nn && d2 > best) best = d2;
        }
    }

    uint64_t m = nn - best;             /* minimal m satisfying the condition */
    return (seq_t)m;
}

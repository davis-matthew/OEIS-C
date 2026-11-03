/*
 * A143324  Table T(n,k) by antidiagonals. T(n,k) is the number of length n
 *          primitive (=aperiodic or period n) k-ary words (n,k >= 1).
 * https://oeis.org/A143324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,k), where (i,k) is the entry of the triangle T(n,k) =
 *            Sum_{d|n} k^d * mu(n/d) in antidiagonal order
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 163, u64 n <= 423,
 *            u128 n <= 1113, big any n
 * Validated: standard, pass, 82 terms through n = 8645 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/ipow.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for zero index */

    /* Find the antidiagonal index s such that (s-1)*s/2 >= n */
    idx_t s = 1;
    while (((s - 1) * s) / 2 < n) ++s;

    /* Count of entries before this diagonal */
    idx_t prev = ((s - 2) * (s - 1)) / 2;

    /* Row and column indices in the triangle */
    idx_t i = n - prev;      /* 1 ≤ i ≤ s-1 */
    idx_t k = s - i;         /* 1 ≤ k ≤ s-1 */

    /* Compute T(i,k) = sum_{d|i} k^d * mu(i/d)  */
    seq_t pos = 0;  /* sum of terms with mu = +1 */
    seq_t neg = 0;  /* sum of terms with mu = -1 */

    for (idx_t d = 1; d <= i; ++d) {
        if (i % d != 0) continue;
        int mu = mu_of(i / d);
        if (mu == 0) continue;
        seq_t val = ipow((seq_t)k, (seq_t)d);
        if (mu > 0)
            pos = pos + val;
        else
            neg = neg + val;
    }

    return pos - neg;   /* final value is non‑negative */
}

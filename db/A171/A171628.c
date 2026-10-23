/*
 * A171628  Number of compositions of n such that the smallest part is
 *          divisible by the number of parts.
 * https://oeis.org/A171628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k>=1} Sum_{t=1}^{⌊n/(k²)⌋} * ( C(n - k²t + k - 1, k
 *            - 1) - [n - k²t ≥ k] * C(n - k²t - 1, k - 1) )
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 133, u64 n <= 353,
 *            u128 n <= 900, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A171628: Number of compositions of n such that the smallest part
 * is divisible by the number of parts.  The implementation uses a
 * multiplicative binomial routine with gcd‑based cancellation to
 * avoid overflow on 64‑bit builds; it works for arbitrary‑precision
 * seq_t as well. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* multiplicative binomial coefficient with gcd reduction.  It works
 * for seq_t being 64‑bit, 128‑bit, or an arbitrary‑precision class. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numerator = (n - k + i);
        seq_t denom     = i;

        /* cancel common factors between numerator and denom */
        seq_t g1 = gcd(numerator, denom);
        numerator /= g1;
        denom     /= g1;

        /* cancel remaining common factors between res and denom */
        seq_t g2 = gcd(res, denom);
        res /= g2;
        denom /= g2;
        /* denom should now be 1 */

        /* multiply using 128‑bit to avoid intermediate overflow on 64‑bit builds */
        unsigned __int128 prod = (unsigned __int128)res * (unsigned __int128)numerator;
        res = (seq_t)prod;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1 */

    seq_t total = 0;

    for (idx_t k = 1; k * k <= n; ++k) {
        idx_t tmax = n / (k * k);
        for (idx_t t = 1; t <= tmax; ++t) {
            idx_t S = n - k * k * t;              /* n - k²t */

            seq_t bin1 = binom(S + k - 1, k - 1);
            seq_t bin2 = 0;
            if (S >= k) bin2 = binom(S - 1, k - 1);

            total += bin1 - bin2;
        }
    }

    return total;
}

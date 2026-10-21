/*
 * A224247  G.f.: Sum_{n>=1} x^n*(1+x)^d(n), where d(n) is the number of
 *          divisors of n (A000005).
 * https://oeis.org/A224247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} C(d(k), n‑k) where d(k) is the number of
 *            divisors of k (OEIS A000005)
 * Range:     u8 n <= 52, u16 n <= 246, u32 n <= 1273, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   _binom_choose
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_binom_choose.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* offset is 1; behaviour below is irrelevant */

    /* Allocate array for divisor counts d[1..n]. */
    idx_t *d = (idx_t *)calloc((size_t)n + 1, sizeof(idx_t));
    if (!d) return 0;                 /* allocation failure – unlikely for n≤10000 */

    /* Compute divisor counts via a simple sieve. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            ++d[j];
        }
    }

    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t r = n - k;               /* choose r = n‑k  */
        if (d[k] >= r) {
            seq_t term = _binom_choose(d[k], r);
            result += term;
        }
    }

    free(d);
    return result;
}

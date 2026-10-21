/*
 * A210097  G.f.: 1 = Sum_{n>=0} a(n)*x^n*(1-x)^(n+1)/(1 + (n+1)*x)^(2*n+1).
 * https://oeis.org/A210097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} -(-1)^k * binomial(2*n-k, k) * a(n-k) *
 *            (n-k+2)^k Initial value: a(0) = 1 (Implementation uses binary
 *            recurrence with memoisation and arbitrary-precision arithmetic
 *            types where needed.)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"   /* binomial(idx_t, idx_t) -> seq_t */
#include "../utils/seq_pow.h"    /* seq_pow(seq_t, idx_t) -> seq_t */

/* Cached values of the sequence. */
static seq_t *seq_cache = NULL;
static idx_t  cache_cap  = 0;  /* number of cached terms (next index to compute) */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Expand cache if necessary. */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        seq_cache = new_cache;

        for (idx_t m = cache_cap; m <= n; ++m) {
            if (m == 0) {
                seq_cache[0] = (seq_t)1;
                continue;
            }

            seq_t sum_pos = 0;
            seq_t sum_neg = 0;

            for (idx_t k = 1; k <= m; ++k) {
                /* sign = (-1)^(k+1) : positive for odd k, negative for even k */
                const int is_positive = (k & 1ULL) ? 1 : 0;

                /* binomial(2*m - k, k) */
                seq_t bin = binomial(2 * m - k, k);

                /* a(m - k) */
                seq_t aval = seq_cache[m - k];

                /* (m - k + 2)^k */
                seq_t base = (seq_t)(m - k + 2);
                seq_t pw   = seq_pow(base, k);

                /* term = bin * aval * pw */
                seq_t term = bin;
                term *= aval;
                term *= pw;

                if (is_positive)
                    sum_pos += term;
                else
                    sum_neg += term;
            }

            seq_cache[m] = sum_pos;
            seq_cache[m] -= sum_neg;   /* final value is non‑negative */
        }

        cache_cap = new_cap;
    }

    return seq_cache[n];
}

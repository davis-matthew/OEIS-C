/*
 * A070544  Number of squarefree numbers s such that n < s < 2n.
 * https://oeis.org/A070544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=n+1}^{2n-1} μ(k)^2 (μ is the Möbius function)
 * Range:     u8 n <= 415, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9718 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* internal routine that builds the Möbius function up to `limit`
   and returns the number of square‑free numbers in (n, 2n).            */
static seq_t squarefree_in_interval(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety net for offset 1 */

    idx_t limit = 2 * n - 1;
    if (limit < 1) return (seq_t)0;       /* should not happen for n≥1 */

    /* Allocate temporary arrays */
    int8_t *mu      = (int8_t*)malloc((limit + 1) * sizeof(int8_t));
    bool   *is_comp = (bool*)calloc((limit + 1), sizeof(bool));
    idx_t  *primes  = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    size_t  primes_cnt = 0;

    mu[0] = 0;
    if (limit >= 1) mu[1] = 1;            /* μ(1) = 1 */

    /* Linear sieve to compute μ(n) for all n ≤ limit */
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            if (i > limit / p) break;
            idx_t prod = i * p;
            is_comp[prod] = true;
            if (i % p == 0) {
                mu[prod] = 0;
                break;
            } else {
                mu[prod] = -mu[i];
            }
        }
    }

    /* Prefix sums of μ^2: pref[i] = number of square‑free ≤ i */
    seq_t *pref = mem_new(seq_t, (limit + 1));
    pref[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t inc = (mu[i] == 0) ? (seq_t)0 : (seq_t)1;
        pref[i] = pref[i-1] + inc;
    }

    seq_t result = pref[limit] - pref[n];   /* count of square‑free in (n,2n) */

    /* Free temporary memory */
    free(mu);
    free(is_comp);
    free(primes);
    mem_free(pref);

    return result;
}

/* Public entry point required by the test harness */
seq_t a(idx_t n)
{
    return squarefree_in_interval(n);
}

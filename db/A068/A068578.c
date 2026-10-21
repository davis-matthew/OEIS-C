/*
 * A068578  Numbers k such that tau(k)*sigma(k) > k*phi(k).
 * https://oeis.org/A068578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) is the nth integer k such that τ(k)*σ(k) > k*φ(k)
 * Range:     u8 n <= 21, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   calc_phi, factor_u64, seq_alloc_new_array, seq_realloc, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

/* Static cache for previously found terms */
static seq_t *terms = NULL;
static size_t terms_len = 0;   /* number of terms currently cached   */
static size_t terms_cap = 0;   /* allocated capacity of terms[]      */
static uint64_t next_k = 1;    /* next integer to examine           */

seq_t a(idx_t n)
{
    /* Scan integers until we have collected n terms */
    while (terms_len < (size_t)n)
    {
        ++next_k;                     /* candidate integer k            */
        seq_t k = (seq_t)next_k;      /* cast for arithmetic            */

        /* Factor k to compute τ(k) (divisor count) */
        size_t  cnt   = 0;
        uint64_t primes[64];
        uint64_t exps  [64];
        factor_u64(next_k, primes, exps, &cnt);

        seq_t tau = 1;
        for (size_t i = 0; i < cnt; ++i)
            tau *= (exps[i] + 1);

        /* σ(k) and φ(k) via provided helpers */
        seq_t sigma = sigma_of(next_k);   /* sum of divisors              */
        seq_t phi   = calc_phi(k);        /* Euler’s totient             */

        /* Inequality check: τ(k)*σ(k) > k*φ(k) */
        if ((tau * sigma) > (k * phi))
        {
            /* Grow cache if necessary */
            if (terms_len == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = seq_realloc(terms, terms_cap, new_cap);
                if (!tmp) exit(1);         /* exhaustion – should not occur */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = k;      /* store found term            */
        }
    }
    return terms[n - 1];                  /* 1‑based indexing           */
}

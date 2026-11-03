/*
 * A068578  Numbers k such that tau(k)*sigma(k) > k*phi(k).
 * https://oeis.org/A068578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) is the nth integer k such that τ(k)*σ(k) > k*φ(k)
 * Range:     u8 n <= 21, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: full, pass, 23 terms through n = 23 (2026-11-03)
 * Helpers:   factor_u64, mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/factor_u64.h"
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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
        seq_t phi   = totient_seq(k);        /* Euler’s totient             */

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

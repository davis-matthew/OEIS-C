/*
 * A122417  Factorials from an irrationality measure for e, with a(1) = 2.
 * https://oeis.org/A122417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A002034(n)+1)!
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 12, u64 n <= 22, u128 n <= 28,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the factorial of the smallest m such that n divides m! (plus one) */

/* External types are supplied by the build */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Helper: exponent of prime p in m! (Legendre's formula) */
static inline uint64_t exp_in_factorial(uint64_t m, uint64_t p)
{
    uint64_t sum = 0;
    while (m >= p) {
        m /= p;
        sum += m;
    }
    return sum;
}

/* Helper: minimal m such that the exponent of p in m! is at least e */
static inline uint64_t minimal_m_for(uint64_t p, uint64_t e)
{
    uint64_t low = 1;
    uint64_t high = e * p;          /* safe upper bound */
    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (exp_in_factorial(mid, p) >= e)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Use the provided factorisation routine */
#include "../utils/factor_u64.h"

/* Compute A002034(n): min m such that n | m! */
static uint64_t compute_S(uint64_t n)
{
    if (n <= 1) return 1;   /* A002034(1) = 1 */

    /* Temporary storage for factorisation */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    uint64_t max_m = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t m = minimal_m_for(p, e);
        if (m > max_m) max_m = m;
    }
    return max_m;
}

/* Cache of factorial values: fact_cache[k] = k! for k >= 0 */
static seq_t *fact_cache = NULL;
static size_t fact_cap  = 0;
static size_t fact_len  = 0;   /* number of stored factorials (first is 0!) */

/* Ensure that factorials up to index k are computed and stored. */
static void ensure_fact_cache(idx_t k)
{
    if (fact_cache == NULL) {                /* initialise */
        fact_cap = 64;
        fact_cache = (seq_t *)malloc(fact_cap * sizeof(seq_t));
        if (!fact_cache) return;             /* allocation failed – ignore here */
        fact_cache[0] = (seq_t)1;            /* 0! = 1 */
        fact_len = 1;
    }

    while (fact_len <= (size_t)k) {
        if (fact_len >= fact_cap) {          /* grow cache */
            fact_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(fact_cache, fact_cap * sizeof(seq_t));
            if (!tmp) return;                /* allocation failed – ignore here */
            fact_cache = tmp;
        }
        seq_t next = fact_cache[fact_len - 1] * (seq_t)fact_len;  /* (n-1)! * n */
        fact_cache[fact_len] = next;
        ++fact_len;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t S = compute_S((uint64_t)n);      /* A002034(n) */
    idx_t fac_idx = (idx_t)(S + 1);           /* we need (S+1)! */
    ensure_fact_cache(fac_idx);
    return fact_cache[fac_idx];
}

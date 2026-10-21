/*
 * A099301  Arithmetic derivative of d(n), the number of divisors of n.
 * https://oeis.org/A099301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic derivative of the divisor count d(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = d'(n),  with d(n) = number of divisors of n (Ω function)          */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* Factorisation routine:  factors a number into primes and exponents.    */
/* --------------------------------------------------------------------- */
static size_t factor(idx_t n, uint64_t *pr, uint64_t *exp)
{
    size_t cnt = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pr[cnt]  = p;
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        pr[cnt]  = tmp;
        exp[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* --------------------------------------------------------------------- */
/* Compute the arithmetic derivative of an integer using its prime      */
/* factorisation:  D(n) = n * Σ e_i / p_i   (e_i exponent of p_i).      */
/* --------------------------------------------------------------------- */
static seq_t derivative_of(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    uint64_t pn[64];
    uint64_t en[64];
    size_t cn = factor(n, pn, en);
    seq_t result = 0;
    for (size_t i = 0; i < cn; ++i) {
        /* term = n * e_i / p_i;  n is divisible by p_i              */
        result += (seq_t)n * (seq_t)en[i] / (seq_t)pn[i];
    }
    return result;
}

/* --------------------------------------------------------------------- */
/* Cached sequential evaluation of a(n). The function is independent   */
/* of call order; it only grows its cache when requested.               */
/* --------------------------------------------------------------------- */
static seq_t* cache = NULL;          /* cache for computed a(n) values   */
static size_t cache_cap = 0;         /* size of allocated cache          */
static idx_t computed = 0;           /* greatest n for which a(n) is known */

/* Grow the cache to hold at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    size_t sc = new_cap * sizeof(seq_t);
    seq_t* newp = (seq_t*)malloc(sc);
    if (!newp) return; /* allocation failure – leave cache unchanged */
    if (cache) {
        memcpy(newp, cache, computed * sizeof(seq_t));
        free(cache);
    }
    /* initialise new entries to 0 */
    memset(newp + computed, 0, (new_cap - computed) * sizeof(seq_t));
    cache = newp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used, but defined for safety */

    /* Extend cache if necessary */
    if (n >= cache_cap) {
        size_t need = n + 1;
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
        while (new_cap <= need) new_cap *= 2;
        grow_cache(new_cap);
    }

    /* Compute missing values up to n */
    if (n > computed) {
        for (idx_t k = computed + 1; k <= n; ++k) {
            /* Step 1: divisor function d(k) */
            uint64_t pk[64];
            uint64_t ek[64];
            size_t ck = factor(k, pk, ek);
            seq_t tau = 1;
            for (size_t i = 0; i < ck; ++i)
                tau *= (seq_t)(ek[i] + 1);        /* τ(k) = Π (e_i+1) */

            /* Step 2: compute derivative of τ(k) */
            seq_t val = derivative_of((idx_t)tau); /* derivative of τ(k) */
            cache[k] = val;
        }
        computed = n;
    }
    return cache[n];
}

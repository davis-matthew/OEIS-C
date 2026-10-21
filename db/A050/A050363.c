/*
 * A050363  Number of ordered factorizations into prime powers greater than 1.
 * https://oeis.org/A050363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p^k|n, p prime, k>=1} a(n / p^k) (a(1)=1)
 * Range:     u8 n <= 383, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Friendly code that memoizes results; uses factor_u64 for prime powers. */

/* Include utils */
#include <stdlib.h>

/* use utility for factorization */
#include "../utils/factor_u64.h"

static seq_t *f_cache   = NULL;   /* cached values of a(n)      */
static bool  *visited   = NULL;   /* flags for computed entries */
static idx_t  capacity  = 0;      /* largest allocated index */

/* Ensure cache large enough for index n (n>=1) */
static void
ensure_capacity(idx_t n)
{
    if (n <= capacity)
        return;

    idx_t new_cap = n;                    /* exact size needed */
    seq_t *new_f  = (seq_t *)calloc(new_cap + 1, sizeof(seq_t));
    bool  *new_v  = (bool  *)calloc(new_cap + 1, sizeof(bool));
    if (!new_f || !new_v) {
        /* allocation failure – abort */
        exit(1);
    }

    /* copy previously computed results */
    if (f_cache) {
        memcpy(new_f, f_cache, (capacity + 1) * sizeof(seq_t));
        free(f_cache);
    }
    if (visited) {
        memcpy(new_v, visited, (capacity + 1) * sizeof(bool));
        free(visited);
    }

    f_cache  = new_f;
    visited  = new_v;
    capacity = new_cap;

    /* base value for a(1) */
    f_cache[1]  = (seq_t)1;
    visited[1]  = true;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0)                 /* sequence defined for n≥1 */
        return (seq_t)0;

    ensure_capacity(n);

    if (visited[n])             /* already computed */
        return f_cache[n];

    /* factor n and sum over all prime‑power divisors */
    uint64_t primes[64], exps[64];
    size_t   cnt = 0;

    factor_u64(n, primes, exps, &cnt);   /* primes[i]^exps[i] | n */

    seq_t sum = (seq_t)0;

    for (size_t i = 0; i < cnt; ++i) {
        idx_t p   = (idx_t)primes[i];
        idx_t exp = (idx_t)exps[i];

        idx_t pow = 1;
        for (idx_t e = 1; e <= exp; ++e) {
            pow = pow * p;                     /* p^e ≤ n */
            seq_t sub = a(n / pow);             /* a(n / p^e) */
            sum = sum + sub;
        }
    }

    f_cache[n] = sum;
    visited[n] = true;
    return sum;
}

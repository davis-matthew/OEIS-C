/*
 * A175065  Smallest number m for which there are exactly n ways to write m as
 *          x^y with x >= 2, y >= 1.
 * https://oeis.org/A175065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^s where s is the least integer having d(s) = n (d(s) =
 *            number of divisors of s)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* utilities provided by the build environment */
#include "../utils/seq_pow.h"   /* seq_t seq_pow(seq_t base, idx_t exp) */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b) */

/* first 15 primes – sufficient for n ≤ 10000
   (the number of prime factors needed for d(s)=n never exceeds 14) */
static const uint32_t primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47
};
static const size_t PRIME_COUNT = sizeof(primes) / sizeof(primes[0]);

/* cache for previously computed minimal s */
static seq_t *cache_s = NULL;
static idx_t   cache_len = 0;

/* ------------------------------------------------------------------ */
/* Recursive search for the smallest s with a given divisor count         */
/* remaining : remaining divisor‑factor product to realise
   last_exp   : largest exponent allowed in the next step
   prime_idx : index of the next prime to use
   cur_s     : product of primes^exponents built so far
   best      : pointer to the current best (smallest) s found         */
static void
dfs_find(idx_t remaining, uint32_t last_exp,
         idx_t prime_idx, seq_t cur_s, seq_t *best)
{
    if (remaining == 1) {
        /* complete factorisation found */
        if (*best == 0 || cur_s < *best)
            *best = cur_s;
        return;
    }
    if (prime_idx >= PRIME_COUNT)
        return;                 /* no more primes – cannot finish */

    /* exponent e must satisfy: e + 1 divides remaining */
    /* and e <= last_exp  (to keep exponents non‑increasing) */
    /* additionally e <= remaining - 1 (since e+1 ≤ remaining)  */
    uint32_t max_e = remaining - 1;
    if (last_exp < max_e)
        max_e = last_exp;

    for (uint32_t e = 1; e <= max_e; ++e) {
        if (remaining % (e + 1) != 0)
            continue;

        /* prime^e */
        seq_t p_pow = seq_pow((seq_t)primes[prime_idx], (idx_t)e);
        /* new candidate s */
        seq_t new_s = seq_mul(cur_s, p_pow);

        /* pruning: if we already have a solution and this path
           is no better, skip it */
        if (*best && new_s >= *best)
            continue;

        dfs_find(remaining / (e + 1), e, prime_idx + 1, new_s, best);
    }
}

/* Find minimal s for a given divisor count n */
static seq_t
minimal_s_for_d(idx_t n)
{
    seq_t best = 0;
    /* initial maximal exponent cannot exceed n-1 */
    dfs_find(n, n > 0 ? (uint32_t)(n - 1) : 0, 0, (seq_t)1, &best);
    return best;
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;         /* out of bounds – return zero */

    /* grow cache if necessary */
    if (n >= cache_len) {
        size_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache_s, new_len * sizeof(seq_t));
        if (!tmp)
            abort();             /* allocation failure */
        cache_s = tmp;
        for (size_t i = cache_len; i < new_len; ++i)
            cache_s[i] = (seq_t)0;
        cache_len = new_len;
    }

    if (cache_s[n] == (seq_t)0)
        cache_s[n] = minimal_s_for_d(n);

    /* the sequence value is 2^s */
    return seq_pow((seq_t)2, (idx_t)cache_s[n]);
}

/*
 * A159063  a(1)=0, a(n+1) is the smallest nonprime >= n^smallest digit of
 *          a(n).
 * https://oeis.org/A159063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   big_pow, is_prime_big, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; for n≥1, a(n+1)= smallest nonprime ≥ n^d
   where d is the smallest decimal digit of a(n). */
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/is_prime_big.h"
#include "../utils/big_pow.h"

/* Return the smallest decimal digit of x (0 if x==0). */
static seq_t min_digit(seq_t x)
{
    if (x == 0)
        return (seq_t)0;
    seq_t best = (seq_t)9;
    while (x > 0) {
        seq_t d = x % (seq_t)10;
        if (d < best)
            best = d;
        x = x / (seq_t)10;
    }
    return best;
}

/* Compute base^exp for small exp (0–9).  exp is an unsigned int. */
static seq_t pow_small(seq_t base, unsigned int exp)
{
    seq_t r = (seq_t)1;
    while (exp) {
        r *= base;
        exp--;
    }
    return r;
}

/* /e  Sequence computation. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache          = NULL;   /* indexed from 1..last */
    static size_t cache_cap      = 0;      /* allocated size */
    static idx_t  cache_last     = 0;      /* highest index computed */

    if (n == 0)   /* safety for under‑offset */
        return (seq_t)0;

    /* ensure cache exists */
    if (cache == NULL) {
        cache_cap = 2;                    /* at least indices 1 and 2 */
        cache = seq_alloc_array(cache_cap);
        cache[1] = (seq_t)0;              /* a(1) */
        cache_last = 1;
    }

    /* enlarge cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = n + 1;
        seq_t *new_cache = seq_alloc_array(new_cap);
        if (cache != NULL)
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i == 1) {            /* a(1) already set */
            cache[1] = (seq_t)0;
            continue;
        }

        seq_t prev = cache[i - 1];
        seq_t d = min_digit(prev);
        seq_t base = (seq_t)(i - 1);      /* use n = i-1 for a(i) */
        seq_t target;
        if (d <= 9) {
            target = pow_small(base, (unsigned int)d);
        } else {
            target = big_pow(base, d);
        }
        seq_t cand = target;
        while (is_prime_big(cand))
            cand++;
        cache[i] = cand;
    }

    cache_last = n;
    return cache[n];
}

/*
 * A130166  a(1)=1; a(n)=prime(mod(a(n-1),1000)).
 * https://oeis.org/A130166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=prime(mod(a(n‑1),1000)) for n≥2.
   Modulus is taken of the previous term, the result is in [0,999].
   0 means the 1000‑th prime.  The sequence is memoised so calls
   may arrive in any order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

static seq_t primes_1000[1000];
static bool primes_ready = false;

/* Build the first 1000 primes (first is 2). */
static void init_prime_table(void)
{
    if (primes_ready)
        return;

    idx_t candidate = 2;
    int count = 0;
    while (count < 1000) {
        if (is_prime(candidate))
            primes_1000[count++] = (seq_t)candidate;
        candidate++;
    }
    primes_ready = true;
}

static seq_t *cache        = NULL;   /* 1‑based: cache[1] = a(1) */
static idx_t  last_computed = 0;     /* largest n already computed */
static seq_t  last_full     = 0;     /* value of a(last_computed) */

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n<1 */

    init_prime_table();

    /* If we already have the term, return it immediately. */
    if (cache != NULL && n <= last_computed)
        return cache[n];

    /* Allocate or grow the cache. */
    if (cache == NULL) {
        size_t cap = (size_t)n + 1;      /* 1-based, need n+1 slots */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;             /* allocation failed */
        cache[1] = (seq_t)1;
        last_computed = 1;
        last_full     = (seq_t)1;
    } else if ((size_t)(n + 1) > (size_t)last_computed + 1) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;             /* reallocation failed */
        cache = tmp;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t mod_val = last_full % (seq_t)1000;   /* 0 ≤ mod_val < 1000 */
        idx_t mod_idx = (idx_t)mod_val;            /* safe cast, small value */
        if (mod_idx == 0)
            mod_idx = 1000;                       /* 1000th prime */
        seq_t next_val = primes_1000[mod_idx - 1];
        cache[i] = next_val;
        last_full = next_val;                    /* update previous value */
    }
    last_computed = n;
    return cache[n];
}

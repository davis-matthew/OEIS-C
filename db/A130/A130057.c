/*
 * A130057  Primes prime(k) such that at least one of the two numbers
 *          (prime(k+1)^2-prime(k)^2)/2 - 1 and (prime(k+1)^2-prime(k)^2)/2 +
 *          1 is prime.
 * https://oeis.org/A130057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A130057: a(n) = the nth prime p_k such that at least one of
   (p_{k+1}^2 – p_k^2)/2 ± 1 is prime. 
   We generate primes on demand using next_prime() from the utils library,
   test the condition with is_prime_big(), and cache the results so that
   subsequent calls are O(1) amortised. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */
#include "../utils/next_prime.h"    /* next_prime(uint64_t) */

/* -------------- static cache state ----------------------------------- */
static seq_t *cache      = NULL;   /* array of found primes          */
static size_t cache_len  = 0;      /* number of elements in cache    */
static size_t cache_cap  = 0;      /* allocated capacity of cache    */
static uint64_t cur_prime = 0;     /* current prime to be examined   */

/* ensure cache can hold at least `need` elements */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) {
        /* allocation failed – abort gracefully */
        abort();
    }
    cache = new_arr;
    cache_cap = new_cap;
}

/* generate qualifying primes until the cache contains at least `target` */
static void generate_until(size_t target)
{
    for (;;) {
        if (cache_len >= target)
            break;

        /* initialise current prime if not yet done */
        if (cur_prime == 0)
            cur_prime = next_prime(2);          /* first odd prime */

        uint64_t p1 = cur_prime;
        uint64_t p2 = next_prime(p1);          /* next consecutive prime */

        seq_t diff = ((seq_t)p2 * (seq_t)p2 - (seq_t)p1 * (seq_t)p1) / 2;

        if (is_prime_big(diff - 1) || is_prime_big(diff + 1)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)p1;
        }

        cur_prime = p2;  /* move to the next pair (p2, next) */
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0 */

    generate_until((size_t)n);
    return cache[(size_t)n - 1];
}

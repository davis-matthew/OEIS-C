/*
 * A090389  Duplicate of A061238.
 * https://oeis.org/A090389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 37, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A090389: primes p such that p ≡ 2 (mod 9) (duplicate of A061238) */
/* The function generates terms on demand by searching successive numbers */
/* congruent to 2 modulo 9 and testing for primality. */

/* Dependencies assumed to be supplied by the build system. */
#include <stdlib.h>
#include <stdbool.h>

/* --- primality test --- */
static bool isPrime(seq_t n)
{
    if (n < 2)        return false;
    if (n == 2)       return true;
    if (n % 2 == 0)   return false;
    for (seq_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

/* --- main sequence function --- */
seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n == 0) return 0;      /* undefined for n<1, safe guard */

    /* grow cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        seq_t candidate;
        if (cache_len == 0)
            candidate = 2;               /* first term */
        else
            candidate = cache[cache_len - 1] + 9; /* next number ≡2 mod 9 */

        /* find the next prime in the progression */
        while (!isPrime(candidate))
            candidate += 9;

        /* extend cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) /* fatal: OOM – let the build fail */
                return 0;
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_len++] = candidate;
    }

    return cache[n - 1];
}

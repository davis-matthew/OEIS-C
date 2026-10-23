/*
 * A141958  Primes congruent to 16 mod 27.
 * https://oeis.org/A141958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime ≡ 16 (mod 27) – computed by incremental search
 * Range:     u8 n <= 3, u16 n <= 366, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */
#include <stdlib.h>   /* for malloc, realloc, free */
#include "../utils/is_prime.h" /* provides bool is_prime(idx_t) */

static void ensure_cache_size(idx_t needed, seq_t **cache, size_t *cap)
{
    if (*cap >= (size_t)needed)
        return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 16;
    while (new_cap < (size_t)needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    *cache = tmp;
    *cap = new_cap;
}

/* Returns the n-th prime congruent to 16 modulo 27 (a(1)=43). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n < 1, return 0 */

    static seq_t *cache = NULL; /* cached primes */
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t cur_candidate = 16; /* last examined candidate */

    while (cache_len < n) {
        cur_candidate += 27;                /* next number ≡ 16 (mod 27) */
        if (is_prime(cur_candidate)) {      /* test primality */
            ensure_cache_size(cache_len + 1, &cache, &cache_cap);
            cache[cache_len++] = (seq_t)cur_candidate;
        }
    }
    return cache[n - 1];
}

/*
 * A173976  Numbers m such that the concatenation of m and 999 is the lesser
 *          of twin primes, i.e., a millennium twin prime couple.
 * https://oeis.org/A173976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth integer m such that concat(m,999) and *
 *            concat(m,999)+2 are twin primes. Computed by searching
 *            candidates * m ≡ 2 (mod 3)
 * Range:     u8 none, u16 n <= 2, u32 n <= 2, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* stored values */
static size_t cache_len = 0;     /* number of cached terms */
static size_t cache_cap = 0;     /* allocated capacity */

/* Grow cache capacity to at least needed elements. */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th value of A173976 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for index 0, but safe */

    /* Ensure the cache has room for at least n entries. */
    ensure_cache_cap((size_t)n);

    /* static state to keep work across calls */
    static seq_t current_m = 2;      /* starts with the first candidate */
    static bool initialized = false;

    if (!initialized) {
        /* current_m is already set to 2, which is ≡2 (mod 3) */
        initialized = true;
    }

    /* Generate candidates until we have at least n terms. */
    while (cache_len < (size_t)n) {
        /* Build p = concat(current_m, 999) = current_m * 1000 + 999. */
        seq_t p = current_m * (seq_t)1000 + (seq_t)999;

        /* Check twin prime condition: p and p+2 must be prime. */
        if (is_prime_big(p) && is_prime_big(p + (seq_t)2)) {
            cache[cache_len++] = current_m;
        }

        /* Next candidate must again be ≡ 2 (mod 3). */
        current_m += (seq_t)3;
    }

    return cache[n - 1];
}

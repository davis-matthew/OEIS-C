/*
 * A229947  Primes congruent to {1, 11, 13, 17, 19, 29} mod 30.
 * https://oeis.org/A229947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   allowed_residue, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to {1,11,13,17,19,29} modulo 30.
   Sequence starts with 11,13,17,19,29,31,41,43,…   */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/allowed_residue.h"

/* static storage for memoized terms */
static seq_t *cache      = NULL;    /* array of cached values      */
static size_t cache_len  = 0;       /* number of cached values     */
static size_t cache_cap  = 0;       /* current allocation size     */
static idx_t  last_start = 11;      /* next candidate to test     */

/* grow the cache to hold at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    } else if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* append a new term to the cache, enlarging if necessary */
static void append_term(seq_t val)
{
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
        grow_cache(new_cap);
    }
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------------ */
/*  Public function required by the template                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* only indices >= 1 are defined */

    /* return cached value if already computed */
    if ((size_t)n <= cache_len) return cache[n - 1];

    /* compute further terms until we reach index n */
    while (cache_len < (size_t)n) {
        /* find next admissible candidate */
        while (true) {
            if (allowed_residue(last_start % 30) && is_prime(last_start))
                break;
            ++last_start;
        }
        append_term((seq_t)last_start);
        ++last_start;                  /* next candidate for other calls   */
    }

    return cache[n - 1];
}

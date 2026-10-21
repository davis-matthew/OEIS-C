/*
 * A056851  Integers n such that the number of digits in n! is a cube.
 * https://oeis.org/A056851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer k (1‑based indexing) such that the
 *            number of decimal digits in k! is a perfect cube. Sequence
 *            starts with k = 0,1,2,3,11,26,83,128,…
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* ===== Cache for sequence terms ===== */
static seq_t *cache      = NULL;   /* array of cached terms */
static size_t cache_size = 0;      /* how many terms are stored */
static size_t cache_cap  = 0;      /* allocated capacity            */

/* ===== State for incremental generation ===== */
static idx_t   current_n  = 0;      /* largest n already processed   */
static long double log_sum = 0.0L;  /* cumulative sum of log10(k);  set up for 0! = 1 */
static uint64_t cube_root = 1;      /* current cube‑root candidate   */

/* Attempt to grow the cache if needed */
static void ensure_capacity(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = seq_alloc_new_array(cache_cap);
    } else if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cache_size; ++i)
            tmp[i] = cache[i];
        seq_free_array(cache);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Extend the cached sequence until it contains at least `target` terms */
static void extend_up_to(size_t target)
{
    /* Insert the very first term, k = 0, the only one for n=0! */
    if (cache_size == 0) {
        ensure_capacity();
        cache[cache_size++] = (seq_t)0;   /* 0! has 1 digit (cube)   */
        current_n = 0;                    /* largest processed n     */
    }

    while (cache_size < target) {
        ++current_n;                                   /* next integer k */

        /* Update running sum of log10(k) for factorial */
        log_sum += log10l((long double)current_n);

        /* Total digits in k! = floor(log10(k!)) + 1 */
        uint64_t digits = (uint64_t)(floorl(log_sum) + 1.0L);

        /* Advance cube_root until cube_root^3 >= digits */
        while ((cube_root * cube_root * cube_root) < digits)
            ++cube_root;

        /* Accept if digits is a perfect cube */
        if ((cube_root * cube_root * cube_root) == digits) {
            ensure_capacity();
            cache[cache_size++] = (seq_t)current_n;
        }
    }
}

/* Public accessor for the sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* out of range, harmless */

    extend_up_to((size_t)n);              /* generate up to n */
    return cache[n - 1];                  /* 1‑based indexing */
}

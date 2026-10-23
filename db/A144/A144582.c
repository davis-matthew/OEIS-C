/*
 * A144582  Numbers having the same leading decimal digit as their cube.
 * https://oeis.org/A144582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k ≥ 0 whose leading decimal digit equals
 *            that of k³ (OEIS A144582)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 797, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: leading decimal digit of a non‑negative seq_t value. */
static seq_t leading_digit(seq_t x)
{
    if (x == (seq_t)0) return 0;
    while (x >= (seq_t)10) x /= (seq_t)10;
    return x;
}

/* Dynamic cache of generated terms */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_len = 0;         /* number of cached terms */
static size_t cache_cap = 0;         /* allocated capacity */
static seq_t next_candidate = (seq_t)0;  /* next integer to test */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Public function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* no value for n = 0, unused */
    if ((size_t)(n-1) < cache_len) return cache[n-1];

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        seq_t k = next_candidate;
        seq_t ld_k   = leading_digit(k);
        seq_t cube   = k * k * k;
        seq_t ld_cube= leading_digit(cube);

        if (ld_k == ld_cube)
        {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = k;
        }
        next_candidate++;
    }
    return cache[n-1];
}

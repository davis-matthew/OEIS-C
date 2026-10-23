/*
 * A153363  Number of zig-zag paths from top to bottom of a rectangle of width
 *          9 with n rows whose color is that of the top right corner.
 * https://oeis.org/A153363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A153363
 *
 * Recurrence: a(1)=5, a(2)=8, a(3)=16, a(4)=28, a(5)=56
 * For n ≥ 6: a(n) = 5*a(n‑2) - 5*a(n‑4)
 * The sequence satisfies the rational gf
 *   G(x) = x*(5+8x-9x²-12x³+x⁴)/(1-5x²+5x⁴).
 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms (1‑indexed) */
static size_t cache_cap = 0;         /* allocated capacity (≥ index used) */
static idx_t   cache_max = 0;        /* largest n whose term is cached */

/* Ensure cache has space for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort();                 // out‑of‑memory, terminate
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of A153363 */
seq_t a(idx_t n)
{
    /* Handle trivial small indices directly (avoids allocating cache for tiny n) */
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)16;
    if (n == 4) return (seq_t)28;
    if (n == 5) return (seq_t)56;

    /* Ensure we have a cache large enough */
    if (n > (idx_t)cache_cap)        /* ensure_cache expects idx_t, using cast */
        ensure_cache(n);

    /* Install base values into cache once, when needed */
    if (cache_max < 5) {
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)28;
        cache[5] = (seq_t)56;
        cache_max = 5;
    }

    /* Compute terms up to n (if not already computed) */
    while (cache_max < n) {
        idx_t i = cache_max + 1;
        /* By recurrence: a(i) = 5*a(i-2) - 5*a(i-4)  for i ≥ 6 */
        seq_t val = (seq_t)5 * cache[i - 2] - (seq_t)5 * cache[i - 4];
        cache[i] = val;
        cache_max = i;
    }

    return cache[n];
}

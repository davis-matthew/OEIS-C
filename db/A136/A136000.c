/*
 * A136000  a(n) = A010814(n) - 1.
 * https://oeis.org/A136000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A136000: a(n) = perimeter of a Pythagorean triple minus 1
   (computed by enumerating all perimeters via Euclid's formula). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* Static cache of computed terms (P‑1). */
static seq_t *cached_terms = NULL;   /* array of terms */
static size_t  cached_len   = 0;     /* number of terms stored          */
static size_t  cached_cap   = 0;     /* allocated capacity              */
static idx_t   current_limit = 0;    /* current perimeter limit         */
static bool    cache_ready   = false;/* whether cache is up to date     */

/* Allocate or enlarge an array for perimeters */
static void _ensure_capacity(size_t needed)
{
    if (needed <= cached_cap)
        return;
    size_t new_cap = cached_cap ? cached_cap * 2 : needed;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_terms = (seq_t *)realloc(cached_terms, new_cap * sizeof(seq_t));
    if (!new_terms) {
        /* Allocation failed – aborting as the build will failure. */
        exit(EXIT_FAILURE);
    }
    cached_terms = new_terms;
    cached_cap   = new_cap;
}

/* Generate all perimeters p <= limit and store (p-1) in the cache. */
static void _generate_terms(idx_t limit)
{
    /* Bitmap of perimeters; 0/1 bytes. */
    size_t sz = (size_t)(limit + 1);
    uint8_t *seen = (uint8_t *)calloc(sz, 1);
    if (!seen)
        exit(EXIT_FAILURE);      /* allocation failure */

    /* Primitive perimeters via Euclid's formula. */
    idx_t m_max = (idx_t)(sqrt((double)limit / 2.0) + 1.0);
    for (idx_t m = 2; m <= m_max; ++m) {
        for (idx_t n = 1; n < m; ++n) {
            if (((m - n) & 1) == 0)    /* same parity – skip */
                continue;
            /* gcd check (small integers) */
            idx_t a = m, b = n;
            while (b != 0) { idx_t t = a % b; a = b; b = t; }
            if (a != 1)
                continue;
            idx_t p0 = 2 * m * (m + n);  /* primitive perimeter */
            if (p0 > limit)
                break;                  /* further n only increase p0 */
            for (idx_t p = p0; p <= limit; p += p0)
                seen[p] = 1;
        }
    }

    /* Count perimeters to determine array size. */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (seen[i])
            ++count;

    _ensure_capacity(count);
    /* Fill cache with sorted terms (P-1). */
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (seen[i])
            cached_terms[pos++] = (seq_t)(i - 1);

    cached_len = pos;
    cache_ready = true;
    free(seen);
}

/* Public function. */
seq_t a(idx_t n)
{
    if (n == 0)         /* undefined for n=0, return 0 */
        return (seq_t)0;

    if (!cache_ready) {         /* initial run */
        current_limit = 200000ULL;   /* enough for the first few thousand terms */
        _generate_terms(current_limit);
    }

    while (n > cached_len) {     /* need more terms */
        current_limit *= 2ULL;   /* bump the limit and regenerate */
        _generate_terms(current_limit);
        if (current_limit > 100000000ULL)  /* safety guard */
            break;
    }

    return cached_terms[n - 1];   /* 1‑based indexing */
}

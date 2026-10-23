/*
 * A022557  Numbers that are not the sum of a square and 2 nonnegative cubes.
 * https://oeis.org/A022557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 9477, u32 n <= 9477, u64 n <= 9477,
 *            u128 n <= 9477, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A022557 – Numbers that are not the sum of a square and 2 nonnegative cubes.
   Algorithm: generate all numbers that can be written as s² + a³ + b³,
   keep a bitmap of representable values, and collect the complementary
   values.  The implementation uses lazy expansion of the bitmap so that
   it can answer queries in any order. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Bitmap: mark[i] == 1 if i can be expressed as s² + a³ + b³. */
static unsigned char *mark = NULL;   /* dynamically expanded */
static size_t      mark_cap = 0;      /* size of the bitmap */

/* Helper to expand the bitmap up to a new upper bound `limit`.  The
   bitmap is enlarged (if necessary) and all representable numbers up to
   `limit` are (re)marked.  This is called whenever the next value to
   test lies beyond the current bitmap. */
static void ensure_limit(idx_t limit)
{
    /* already large enough? */
    if ((size_t)limit < mark_cap)
        return;

    size_t new_cap = (size_t)limit + 1;
    unsigned char *old = mark;
    mark = (unsigned char *)realloc(mark, new_cap);
    if (!mark) {
        /* Allocation failure – terminate the program. */
        exit(1);
    }

    /* Zero the newly allocated portion. */
    if (old == NULL)
        memset(mark, 0, new_cap);
    else
        memset(mark + mark_cap, 0, new_cap - mark_cap);
    mark_cap = new_cap;

    /* Mark all representable numbers <= limit. */
    for (idx_t a = 0; ; ++a) {
        /* safety check: avoid overflow in a³ */
        if (a != 0 && a > limit / a / a)
            break;
        idx_t a3 = a * a * a;

        if (a3 > limit)
            break;

        for (idx_t b = 0; ; ++b) {
            if (b != 0 && b > limit / b / b)
                break;
            idx_t b3 = b * b * b;
            idx_t sum_ab = a3 + b3;
            if (sum_ab > limit)
                break;

            for (idx_t s = 0; ; ++s) {
                if (s != 0 && s > limit / s)
                    break;
                idx_t s2 = s * s;
                idx_t val = sum_ab + s2;
                if (val > limit)
                    break;
                mark[(size_t)val] = 1;
            }
        }
    }
}

/* Cache of the sequence terms.  The cache is expanded lazily. */
static seq_t *cache         = NULL;   /* array of sequence values */
static size_t cache_cap     = 0;      /* allocated size of cache */
static size_t cache_size    = 0;      /* number of stored terms */
static idx_t  next_value    = 1;      /* next integer to test for inclusion */

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)               /* a(0) is undefined – return 0 for safety */
        return 0;

    /* Grow the cache until we have at least `n` terms. */
    while (cache_size < (size_t)n) {
        /* Ensure the bitmap covers the value we are going to test. */
        if ((idx_t)next_value >= (idx_t)mark_cap) {
            idx_t new_limit = next_value * 2;
            if (new_limit <= next_value)   /* overflow protection */
                new_limit = next_value + 1000;
            ensure_limit(new_limit);
        }

        /* If the number is not representable, store it. */
        if (mark[(size_t)next_value] == 0) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache)
                    exit(1);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_value;
        }
        ++next_value;
    }

    return cache[n - 1];
}

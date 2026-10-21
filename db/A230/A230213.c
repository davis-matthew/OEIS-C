/*
 * A230213  Odious numbers (A000069) which can be written as a product of two
 *          evil numbers (A001969).
 * https://oeis.org/A230213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 632, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for odious numbers that are a product of two evil numbers. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* helper: parity of popcount of a seq_t value */
static int popcount_parity(seq_t x)
{
    int p = 0;
    while (x) {
        x &= (x - 1);      /* drop lowest set bit */
        p ^= 1;
    }
    return p;
}

static bool is_evil(seq_t x)   /* even popcount */
{
    return popcount_parity(x) == 0;
}

static bool is_odious(seq_t x) /* odd popcount */
{
    return popcount_parity(x) != 0;
}

/* seq_t a(idx_t n) : nth term of A230213 */
seq_t a(idx_t n)
{
    /* static cache for generated terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;

    /* static list of evil numbers up to current sqrt */
    static seq_t *evil_list = NULL;
    static size_t evil_len  = 0;
    static size_t evil_cap  = 0;
    static idx_t  evil_last = 0;   /* last integer checked for evil */

    /* incremental current number */
    static idx_t cur = 1;

    /* generate terms until we have at least n terms */
    while (cache_size < n) {
        seq_t x = (seq_t)cur++;
        if (!is_odious(x))
            continue;            /* only odious numbers can qualify */

        /* keep evil_list populated up to sqrt(x) */
        seq_t s = floor_sqrt(x);
        idx_t lim = (idx_t)s;      /* sqrt small enough for idx_t */
        if (lim > evil_last) {
            for (idx_t i = evil_last + 1; i <= lim; ++i) {
                seq_t val = (seq_t)i;
                if (is_evil(val)) {
                    if (evil_len == evil_cap) {
                        evil_cap = evil_cap ? evil_cap * 2 : 8;
                        evil_list = (seq_t *)realloc(evil_list, evil_cap * sizeof(seq_t));
                    }
                    evil_list[evil_len++] = val;
                }
            }
            evil_last = lim;
        }

        /* test whether x can be written as product of two evil numbers */
        bool good = false;
        for (size_t i = 0; i < evil_len; ++i) {
            seq_t d = evil_list[i];
            if (d > s) break;
            if (x % d == 0) {
                seq_t q = x / d;
                if (is_evil(q)) {
                    good = true;
                    break;
                }
            }
        }

        if (good) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = x;
        }
    }

    return cache[n - 1];
}

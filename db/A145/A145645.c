/*
 * A145645  Numbers k such that (binary weight of k)/(length of binary
 *          representation of k) equals 1/3.
 * https://oeis.org/A145645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth smallest positive integer k such that popcount(k) /
 *            bitlen(k) = 1/3. The sequence is sorted in increasing order
 * Range:     u8 n <= 6, u16 n <= 1200, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* -------------------------------------------------------------------------- */
/*  Cache for generated terms.  All terms are stored in an array so that */
/*  subsequent calls can retrieve previously computed values in O(1).      */
/* -------------------------------------------------------------------------- */

/* static cache variables – internal linkage */
static seq_t *cache     = NULL;   /* array of generated terms      */
static size_t cache_len = 0;      /* number of terms stored       */
static size_t cache_cap = 0;      /* current capacity of cache    */

/* Ensure the cache has space for at least `need` elements.                */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Store a new value at the end of the cache.                               */
static void store_value(seq_t val)
{
    ensure_cache(cache_len + 1);
    cache[cache_len++] = val;
}

/* Retrieve the value at position `idx` (0‑based).                          */
static seq_t retrieve_value(idx_t idx)
{
    return cache[idx];
}

/* -------------------------------------------------------------------------- */
/*  Main sequence function.  Implements the generation of all numbers with   */
/*  binary length 3m and exactly m ones, enumerated for increasing m and   */
/*  within each m in lexicographic order of the lower (m-1) bits.            */
/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* How many terms have already been generated? */
    static idx_t gen_len = 0;   /* size_t would work too, but idx_t matches n */
    /* Current number of ones (m).  Starts at 1. */
    static idx_t m = 1;
    /* Mask of the (m‑1) lower ones inside the current length L‑1 bits. */
    static seq_t lower_mask = 0;
    /* First‑time initialisation guard. */
    static bool first = true;

    if (first) {
        first = false;
        if (m > 1)
            lower_mask = ((seq_t)1 << (m - 1)) - 1;   /* lowest (m‑1) ones */
        else
            lower_mask = 0;                          /* only the leading 1 */
    }

    while (gen_len < n) {
        /* Current bit length L = 3m. */
        idx_t L = 3 * m;
        seq_t value;

        if (m == 1) {                     /* only the leading 1 exists */
            value = ((seq_t)1 << (L - 1));
        } else {
            /* Leading 1 at bit L-1, plus the lower_mask. */
            value = ((seq_t)1 << (L - 1)) | lower_mask;
        }

        store_value(value);
        ++gen_len;

        /* Advance to the next term. */
        if (m == 1) {                     /* only one term for m = 1      */
            ++m;
            lower_mask = ((seq_t)1 << (m - 1)) - 1;
        } else {
            /* Next combination of (m‑1) ones in L‑1 bits. */
            seq_t x      = lower_mask & -lower_mask;      /* lowest set bit */
            seq_t y      = lower_mask + x;                /* propagate carry */
            seq_t next   = y | (((y ^ lower_mask) / x) >> 2);

            /* If the new mask exceeds the lower L‑1 bits, we finished this m. */
            idx_t bits = L - 1;
            if (next >> bits) { /* overflow – start next m */
                ++m;
                lower_mask = ((seq_t)1 << (m - 1)) - 1;
            } else {
                lower_mask = next;
            }
        }
    }

    return retrieve_value(n - 1);   /* indices are 1‑based */
}

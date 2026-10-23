/*
 * A039082  Numbers whose base-9 representation has the same number of 0's and
 *          6's.
 * https://oeis.org/A039082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-9 representation has the same number of 0's and 6's.
   This implementation searches for the next qualifying number when needed. */
#include <stdlib.h>
#include <stdbool.h>

/* Dynamically grow the cache of terms that satisfy the property. */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_cap   = 0;       /* current allocated size of cache */
static size_t cache_size  = 0;       /* number of terms stored in cache */

/* Next integer to test for inclusion. */
static seq_t last_checked = 1;

/* Return true if x has the same number of 0's and 6's in base‑9. */
static bool balanced_in_base9(seq_t x)
{
    unsigned zeros = 0, sixes = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        int r = (int)(tmp % 9);     /* safe: remainder < 9 */
        if (r == 0) zeros++;
        else if (r == 6) sixes++;
        tmp /= 9;
    }
    return zeros == sixes;
}

/* Enlarge the cache array when necessary. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* allocation failure: stop execution */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of sequence A039082. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n = 0 */

    /* Extend the cache until we have the n‑th term. */
    while (cache_size < (size_t)n) {
        if (balanced_in_base9(last_checked)) {
            if (cache_size == cache_cap) grow_cache();
            cache[cache_size++] = last_checked;
        }
        last_checked++;
    }
    return cache[n - 1];
}

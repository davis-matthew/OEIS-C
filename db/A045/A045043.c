/*
 * A045043  Numbers whose base-4 representation contains exactly one 0 and one
 *          2.
 * https://oeis.org/A045043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   brute‑force search for numbers whose base‑4 representation
 *            contains exactly one 0 and exactly one 2
 * Range:     u8 n <= 45, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Cached values of the sequence */
static seq_t *cache = NULL;          /* array of computed terms      */
static size_t cache_cap = 0;         /* current capacity             */
static idx_t cache_len = 0;          /* number of terms stored       */
static idx_t next_candidate = 1;     /* smallest integer not yet examined */

/* Ensure the cache can hold at least 'needed' elements */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < needed) new_cap = needed;
        /* cast to seq_t * is needed to compile as C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the nth element (offset 1) of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined for n == 0, though not part of the sequence */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        idx_t m = next_candidate;

        /* Count digits 0 and 2 in the base‑4 representation of m */
        unsigned zeros = 0, twos = 0;
        idx_t t = m;
        while (t > 0) {
            unsigned digit = (unsigned)(t & 3);  /* t % 4 */
            if (digit == 0) zeros++;
            else if (digit == 2) twos++;
            t >>= 2;                              /* t /= 4 */
        }

        if (zeros == 1 && twos == 1) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len] = (seq_t)m;
            cache_len++;
        }

        next_candidate++;
    }

    return cache[n - 1];
}

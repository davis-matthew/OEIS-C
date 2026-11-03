/*
 * A145649  Characteristic function of the lucky numbers.
 * https://oeis.org/A145649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a lucky number (A000959), 0 otherwise
 * Range:     u8 n <= 100005, u16 n <= 100005, u32 n <= 100005,
 *            u64 n <= 100005, u128 n <= 100005, big any n
 * Validated: standard, pass, 121 terms through n = 9816 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation sieves the lucky numbers up to the largest n requested. */

#include <stdbool.h>
#include <stdlib.h>

/* Build lucky numbers up to max_n, setting lucky_cache[n] = true for lucky n. */
static void build_lucky_up_to(idx_t max_n, bool *is_lucky)
{
    /* We work only with odd numbers: 1,3,5,... ≤ max_n */
    idx_t max_odds = (max_n + 1) / 2;
    idx_t *list = (idx_t *)malloc(max_odds * sizeof(idx_t));
    if (!list) return;          /* allocation failure, leave cache unchanged */

    idx_t size = 0;
    for (idx_t x = 1; x <= max_n; x += 2)
        list[size++] = x;

    /* Sieve using the lucky number steps */
    idx_t i = 1;                 /* start with the second element (value 2) */
    while (i < size) {
        idx_t step = list[i];
        if (step < 2) break;
        if (step > size) break;

        idx_t new_size = 0;
        idx_t count = 1;
        for (idx_t j = 0; j < size; ++j, ++count) {
            if (count % step != 0)
                list[new_size++] = list[j];
        }
        size = new_size;
        ++i;
    }

    /* Mark lucky numbers in the cache */
    for (idx_t j = 0; j < size; ++j)
        is_lucky[list[j]] = true;

    free(list);
}

/* Forward declaration of the helper */
static void build_lucky_up_to(idx_t max_n, bool *is_lucky);

/* Cached sieve data */
static idx_t cache_size = 0;
static bool *lucky_cache = NULL;

/* Return 1 if n is lucky, 0 otherwise. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Rebuild or extend the sieve if necessary */
    if (n > cache_size) {
        /* Reallocate boolean mask */
        bool *new_cache = (bool *)realloc(lucky_cache,
                            (n + 1) * sizeof(bool));
        if (!new_cache) {                /* allocation failure fallback */
            if (lucky_cache) free(lucky_cache);
            lucky_cache = NULL;           /* no cache, just return */
            return (seq_t)0;
        }
        lucky_cache = new_cache;
        for (idx_t i = 1; i <= n; ++i)
            lucky_cache[i] = false;      /* initialise to false */

        build_lucky_up_to(n, lucky_cache);
        cache_size = n;
    }

    return lucky_cache[n] ? (seq_t)1 : (seq_t)0;
}

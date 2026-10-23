/*
 * A163576  Near-factorions: equal to the sum of the factorials of all but one
 *          of their digits in base 10.
 * https://oeis.org/A163576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn, bref
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Near-factorions: numbers equal to the sum of the factorials of all but one of their decimal digits */
#include <stdlib.h>

/* Precomputed factorials for digits 0–9 (type seq_t) */
static const seq_t fact[10] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};

/* Cache of discovered near‑factorions */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static idx_t next_search = 0;    /* last integer tested */

/* Append a value to the cache, expanding it if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* abort on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the n‑th near‑factorion (a(1)=24). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;    /* undefined for n <= 0 */

    /* Grow cache until it contains at least n terms. */
    while (cache_len < n) {
        ++next_search;
        seq_t x = (seq_t)next_search;

        /* Sum of factorials of all digits of x. */
        seq_t sum = 0;
        seq_t tmp = x;
        while (tmp != (seq_t)0) {
            int d = (int)(tmp % (seq_t)10);
            sum += fact[d];
            tmp /= (seq_t)10;
        }

        /* Check if removing one digit's factorial equals the number. */
        tmp = x;
        int found = 0;
        while (tmp != (seq_t)0 && !found) {
            int d = (int)(tmp % (seq_t)10);
            if (x == sum - fact[d]) found = 1;
            tmp /= (seq_t)10;
        }

        if (found) cache_append(x);
    }

    return cache[n - 1];
}

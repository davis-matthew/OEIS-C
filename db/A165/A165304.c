/*
 * A165304  a(n) is the smallest number not already in the sequence, such that
 *          the concatenation of all a(n) displays the periodic digit string
 *          1, 2, 3, 4, 5, 6 (and repeat).
 * https://oeis.org/A165304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the smallest positive integer not already in the
 *            sequence such that, when all a(k) terms are concatenated, the
 *            resulting decimal string is the infinite repetition of
 *            1,2,3,4,5,6. This implementation builds terms one by one,
 *            storing generated values in a resizable cache. For each new
 *            term...
 * Range:     u8 n <= 13, u16 n <= 30, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* pattern digits */
static const int pattern[6] = {1, 2, 3, 4, 5, 6};

/* dynamic cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;

/* current position in the repeating pattern (0 … 5) */
static int pos = 0;

/* ensure cache has space for at least needed elements */
static void ensure_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* if reallocation fails, the old cache remains valid */
    cache        = new_cache;
    cache_cap    = new_cap;
}

/* add a new term to the cache */
static void add_term(seq_t val)
{
    ensure_cache(cache_size + 1);
    cache[cache_size++] = val;
}

/* linear search: check whether val has already been used */
static bool used(seq_t val)
{
    for (size_t i = 0; i < cache_size; ++i)
        if (cache[i] == val)
            return true;
    return false;
}

/* compute the next term and update global state */
static void generate_next(void)
{
    /* try increasing pattern‑substring lengths until an unused
       candidate is found                                  */
    for (int len = 1; ; ++len) {
        seq_t cand = 0;
        int p = pos;
        for (int i = 0; i < len; ++i) {
            cand = cand * (seq_t)10 + (seq_t)pattern[p];
            p = (p + 1) % 6;
        }
        if (!used(cand)) {
            add_term(cand);
            pos = (pos + len) % 6;
            return;
        }
    }
}

/* external function required by the judge */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n<1 */

    /* generate terms up to the requested index */
    while (n > cache_size)
        generate_next();

    return cache[n - 1];
}

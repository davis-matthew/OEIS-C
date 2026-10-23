/*
 * A032983  Numbers with the property that all pairs of consecutive base-6
 *          digits differ by more than 2.
 * https://oeis.org/A032983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A032983: numbers whose base‑6 digits differ by more than 2 */

/* standard headers – necessary for malloc/realloc */
#include <stdlib.h>
#include <stddef.h>

/* local static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static idx_t last_gen_len = 0;

/* append a value to the cache, growing it as needed */
static void append_cache(seq_t val)
{
    if (cache_size == cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            exit((int)EXIT_FAILURE);   /* out of memory – terminate */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* recursive generator for numbers of a given base‑6 length */
static void gen_rec(idx_t len, idx_t pos, seq_t val, int prev_digit)
{
    if (pos == len)
    {
        append_cache(val);
        return;
    }

    int start = (pos == 0) ? 1 : 0;   /* no leading zero for the first digit */
    for (int d = start; d <= 5; ++d)
    {
        if (pos > 0)
        {
            int diff = d > prev_digit ? d - prev_digit : prev_digit - d;
            if (diff <= 2)   /* must be larger than 2 */
                continue;
        }
        gen_rec(len, pos + 1, val * 6 + (seq_t)d, d);
    }
}

/* generate all numbers with base‑6 length = len */
static void generate_len(idx_t len)
{
    gen_rec(len, 0, (seq_t)0, -1);
}

/* requested function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* sequence defined for n ≥ 1 */

    /* generate until we have at least n entries in the cache */
    while (cache_size < n)
    {
        ++last_gen_len;
        generate_len(last_gen_len);
    }
    return cache[n - 1];
}

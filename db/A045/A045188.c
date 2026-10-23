/*
 * A045188  Numbers whose base-5 representation contains no 0's and exactly
 *          one 3.
 * https://oeis.org/A045188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the numbers whose base‑5 representation contains no 0's
 * and exactly one 3.  The sequence is generated lexicographically by
 * length, which preserves numerical order. */
#include <stdlib.h>
#include <stdbool.h>

/* Cache of already generated terms. */
static seq_t *cache = NULL;
static size_t cache_used = 0;
static size_t cache_cap   = 0;
static idx_t current_len  = 0;      /* how many lengths have been generated */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a value to the cache. */
static void append_to_cache(seq_t val)
{
    ensure_cache_size(cache_used + 1);
    cache[cache_used++] = val;
}

/* Recursively generate numbers of a fixed length, keeping track of
 * whether a 3 has already been used.  Only numbers that contain
 * exactly one 3 are appended. */
static void generate_recursive(idx_t pos, idx_t len, seq_t value, bool used3)
{
    if (pos == len)
    {
        if (used3) append_to_cache(value);
        return;
    }
    seq_t base = value * 5;
    for (int d = 1; d <= 4; ++d)
    {
        if (d == 3)
        {
            if (!used3)            /* allow a single 3 only */
                generate_recursive(pos + 1, len, base + 3, true);
        }
        else
            generate_recursive(pos + 1, len, base + d, used3);
    }
}

/* Generate all numbers of the given length and append to cache. */
static void generate_len(idx_t len)
{
    generate_recursive(0, len, 0, false);
}

/* Public function: a(n) returns the nth element (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1, harmless */
    while (cache_used < n)
    {
        generate_len(++current_len);
    }
    return cache[n - 1];
}

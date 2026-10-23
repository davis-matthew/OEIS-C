/*
 * A045026  Numbers whose base-4 representation contains exactly two 0's and
 *          three 1's.
 * https://oeis.org/A045026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base-4 representation contains exactly two 0's and
 *            three 1's
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for the sequence values. */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static idx_t processed_len = 4;  /* largest length processed so far */
static size_t cumulative = 0;    /* total number of terms generated */

/* Ensure the cache has enough capacity for 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Append a value to the cache. */
static void push_value(seq_t v)
{
    ensure_cache_capacity(cache_size + 1);
    cache[cache_size++] = v;
    ++cumulative;
}

/* Recursive generation of numbers of a fixed length L. */
static void generate_rec(idx_t pos, idx_t zeros_left, idx_t ones_left,
                         seq_t val, idx_t L)
{
    if (pos == L) {
        if (zeros_left == 0 && ones_left == 0)
            push_value(val);
        return;
    }

    idx_t rem = L - pos - 1;          /* remaining free positions */

    /* try digits in increasing order to maintain lexicographic order */
    for (int d = 0; d <= 3; ++d) {
        if (pos == 0 && d == 0) /* leading zero not allowed */ continue;

        idx_t z = zeros_left;
        idx_t o = ones_left;
        if (d == 0) {
            if (z == 0) continue;
            --z;
        } else if (d == 1) {
            if (o == 0) continue;
            --o;
        }

        /* after placing d we must be able to fill the remaining positions */
        if (z + o > rem)
            continue;

        generate_rec(pos + 1, z, o, val * 4 + (seq_t)d, L);
    }
}

/* Generate all numbers of a given length L and append them to the cache. */
static void generate_length(idx_t L)
{
    generate_rec(0, (idx_t)2, (idx_t)3, (seq_t)0, L);
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset < 1, return 0 */

    /* Generate length blocks until we have enough terms. */
    while (cumulative < (size_t)n) {
        ++processed_len;
        generate_length(processed_len);
    }

    return cache[(size_t)n - 1];
}

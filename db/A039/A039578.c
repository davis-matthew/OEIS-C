/*
 * A039578  Numbers whose base-6 representation has the same number of 0's,
 *          2's and 4's.
 * https://oeis.org/A039578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-6 representation has the same number of 0's,
 *            2's and 4's
 * Range:     u8 n <= 45, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t generated = 0;
static seq_t next_candidate = 1;

static void ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }
}

static bool is_valid(seq_t x)
{
    unsigned int c0 = 0, c2 = 0, c4 = 0;
    seq_t t = x;
    while (t > (seq_t)0) {
        seq_t d = t % (seq_t)6;
        if (d == (seq_t)0) ++c0;
        else if (d == (seq_t)2) ++c2;
        else if (d == (seq_t)4) ++c4;
        t = t / (seq_t)6;
    }
    return (c0 == c2) && (c2 == c4);
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    while (generated < n) {
        while (!is_valid(next_candidate)) next_candidate++;
        ensure_cache_cap(generated + 1);
        cache[generated++] = next_candidate;
        next_candidate++;
    }
    return cache[(size_t)(n - 1)];
}

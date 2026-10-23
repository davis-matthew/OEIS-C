/*
 * A032967  Numbers with the property that all pairs of consecutive base-4
 *          digits differ by more than 1.
 * https://oeis.org/A032967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   absdiff_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A032967 – numbers whose base‑4 digits have no two consecutive
   digits whose difference is ≤1.  The function generates the sequence in
   ascending order by length and lexicographic digit order. */

#include <stdlib.h>
#include "../utils/absdiff_int.h"

/* Persistent cache of computed terms */
static seq_t *cache = NULL;   /* array of seq_t values */
static size_t cap  = 0;       /* current capacity of cache */
static idx_t term_cnt = 0;    /* number of terms stored */

/* Number of digits of the last generated length.
   The next generation will produce numbers of length curr_len+1. */
static idx_t curr_len = 0;

/* Append a new term to the cache, enlarging the array if needed. */
static void push_cache(seq_t val)
{
    if (term_cnt == cap) {
        size_t new_cap = cap ? (cap << 1) : 8;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(1);   /* allocation failed */
        cache = new_ptr;
        cap = new_cap;
    }
    cache[term_cnt++] = val;
}

/* Absolute difference for int values. */

/* Recursive generation of all valid numbers of a given length.
   current – value built so far (in base 4),
   pos      – number of digits currently in current,
   len      – target number of digits,
   prev     – last digit placed. */
static void dfs_generate(seq_t current, int pos, int len, int prev)
{
    if (pos == len) {
        push_cache(current);
        return;
    }
    for (int nd = 0; nd < 4; ++nd) {
        if (absdiff_int(prev, nd) > 1) {
            dfs_generate((current << 2) + (seq_t)nd, pos + 1, len, nd);
        }
    }
}

/* Generate all numbers of exactly 'len' digits that satisfy the rule
   and append them to the cache. */
static void generate_len(int len)
{
    for (int first = 1; first < 4; ++first) {
        if (len == 1) {
            push_cache((seq_t)first);
        } else {
            dfs_generate((seq_t)first, 1, len, first);
        }
    }
}

/* Ensure that at least 'n' terms are available in the cache.  New lengths
   are generated incrementally. */
static void generate_up_to(idx_t n)
{
    while (term_cnt < n) {
        ++curr_len;
        generate_len((int)curr_len);
    }
}

/* Required function: a(n) = nth term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not used, but harmless */
    generate_up_to(n);
    return cache[n - 1];
}

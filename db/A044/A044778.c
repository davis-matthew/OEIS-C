/*
 * A044778  Numbers n such that string 6,5 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A044778: numbers n whose decimal
   representation contains the substring "65" but n+1 does not. */

#include <stdbool.h>
#include <stdlib.h>

/* Memoisation cache for the sequence */
static seq_t *seq_cache     = NULL;
static size_t seq_cache_len = 0;
static size_t seq_cache_cap = 0;

/* Ensure the cache has capacity for at least 'needed' elements. */
static void seq_cache_ensure_cap(size_t needed)
{
    if (needed <= seq_cache_cap)
        return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 128;
    if (new_cap < needed)
        new_cap = needed;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache_cap = new_cap;
}

/* Return true if decimal representation of x contains the substring "65". */
static bool has65(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 10);
        if (d == 6 && prev == 5)
            return true;
        prev = d;
        x /= 10;
    }
    return false;
}

/* Generate the next term of the sequence and append it to the cache. */
static void generate_next(void)
{
    seq_t cand = seq_cache_len ? seq_cache[seq_cache_len - 1] + 1 : 1;
    while (1) {
        if (has65(cand) && !has65(cand + 1)) {
            seq_cache_ensure_cap(seq_cache_len + 1);
            seq_cache[seq_cache_len++] = cand;
            break;
        }
        ++cand;
    }
}

/* External entry point required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 – return 0 for undefined case */
        return 0;
    while (seq_cache_len < (size_t)n)
        generate_next();
    return seq_cache[(size_t)n - 1];
}

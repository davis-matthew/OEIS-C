/*
 * A178367  Numbers with rounded up arithmetic mean of digits = 7.
 * https://oeis.org/A178367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose rounded‑up arithmetic mean of decimal digits = 7.
   The routine enumerates all positive integers satisfying
        6*digits < sum_of_digits ≤ 7*digits
   and returns the n‑th smallest.  It keeps a static cache, so subsequent
   calls may be in any order. */

#include <stddef.h>
#include <stdlib.h>

/* static cache of generated terms */
static seq_t *cache = NULL;     /* array of terms */
static size_t cache_len = 0;    /* number of terms stored */
static size_t cache_cap = 0;    /* allocated capacity */
static idx_t last_gen_len = 0;  /* last length that has been fully generated */

/* bounds for the current length (set by gen_len) */
static uint64_t cur_low, cur_high;

/* ensure enough capacity for the cache */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* depth‑first search for numbers of the current length */
static void rec_gen(idx_t pos, seq_t cur, uint64_t sum, idx_t len)
{
    if (pos == len) {                          /* full number built */
        if (sum >= cur_low && sum <= cur_high) {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = cur;
        }
        return;
    }

    int start_d = (pos == 0) ? 1 : 0;
    idx_t remaining = len - pos - 1;
    for (int d = start_d; d <= 9; ++d) {
        uint64_t new_sum = sum + (uint64_t)d;
        uint64_t min_possible = new_sum;                 /* +0 per remaining pos */
        uint64_t max_possible = new_sum + 9 * remaining; /* +9 per remaining pos */

        if (max_possible < cur_low)   /* cannot reach low bound */
            continue;
        if (min_possible > cur_high)  /* already above high bound */
            continue;

        seq_t new_cur = cur * (seq_t)10 + (seq_t)d;
        rec_gen(pos + 1, new_cur, new_sum, len);
    }
}

/* generate all numbers of a given length that satisfy the sum range */
static void gen_len(idx_t len)
{
    cur_low  = 6 * len + 1;
    cur_high = 7 * len;
    rec_gen(0, (seq_t)0, 0, len);
}

/* required function */
seq_t a(idx_t n)
{
    if (n == 0)         /* undefined for non‑positive index */
        return (seq_t)0;

    /* generate more terms if needed */
    if (cache_len < (size_t)n) {
        idx_t len = last_gen_len + 1;
        while (cache_len < (size_t)n) {
            gen_len(len);
            last_gen_len = len;
            ++len;
        }
    }

    return cache[(size_t)n - 1];
}

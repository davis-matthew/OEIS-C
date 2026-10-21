/*
 * A086956  a(1)=1, for n>1: a(n) is the smallest divisor of n occurring
 *          earlier at most twice.
 * https://oeis.org/A086956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A086956: a(1)=1; for n>1, a(n) = smallest divisor of n that has appeared
   at most twice among a(1)..a(n-1).  This implementation builds the
   sequence incrementally, caching both the values and the occurrence counts. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Cached sequence values, 1‑based (a_cache[0] = a(1)). */
static seq_t *a_cache = NULL;
static size_t a_cap = 0;        /* capacity of a_cache (≥ a_len) */
static size_t a_len = 0;        /* number of values already computed */

/* Cache of occurrence counts for each possible divisor value.  0‑index unused. */
static uint32_t *cnt_cache = NULL;
static size_t cnt_cap = 0;      /* capacity of cnt_cache (≥ max_value+1) */

/* Ensure a_cache can hold indices up to need-1. */
static void grow_a(size_t need)
{
    if (need <= a_cap)
        return;
    size_t new_cap = a_cap ? a_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;
    a_cap = new_cap;
}

/* Ensure cnt_cache can index values up to need-1, zero‑initialising new slots. */
static void grow_cnt(size_t need)
{
    if (need <= cnt_cap)
        return;
    size_t old_cap = cnt_cap;
    size_t new_cap = cnt_cap ? cnt_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(cnt_cache, new_cap * sizeof(uint32_t));
    if (!tmp) abort();
    if (old_cap < new_cap)
        memset(tmp + old_cap, 0, (new_cap - old_cap) * sizeof(uint32_t));
    cnt_cache = tmp;
    cnt_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n<1, return 0 safely */

    /* Grow caches to accommodate n. */
    grow_a((size_t)n + 1);        /* indices 0..n (a_cache[n-1] = a(n)) */
    grow_cnt((size_t)n + 1);      /* indices 0..n for divisor counts */

    /* Compute values up to n if not already done. */
    while (a_len < n) {
        idx_t ii = a_len + 1;     /* current integer whose value we compute */

        /* Find the smallest divisor d of ii with cnt[d] <= 2. */
        idx_t min_d = ii;          /* default to ii itself */
        for (idx_t d = 1; d * d <= ii; ++d) {
            if (ii % d == 0) {
                /* divisor d */
                if (cnt_cache[d] <= 2 && d < min_d)
                    min_d = d;
                /* paired divisor ii/d */
                idx_t d2 = ii / d;
                if (d2 != d && cnt_cache[d2] <= 2 && d2 < min_d)
                    min_d = d2;
            }
        }

        /* Store the result and update its occurrence count. */
        a_cache[a_len] = (seq_t)min_d;

        if (min_d + 1 > cnt_cap)   /* grow count array if necessary */
            grow_cnt((size_t)min_d + 1);

        cnt_cache[min_d]++;        /* record one more occurrence */

        a_len++;
    }

    return a_cache[n - 1];
}

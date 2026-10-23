/*
 * A043936  Numbers k such that 0 and 5 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: A043936 – numbers k where the base‑8 representation
   contains the digits 0 and 5 next to each other
   (in either order), but the base‑8 representation of k+1 does not.    
   The implementation scans integers and memoises the found terms. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Detect whether a number contains "05" or "50" as adjacent base‑8 digits. */
static bool has05(seq_t v)
{
    if (v == (seq_t)0) return false;

    /* Store base‑8 digits starting with the least significant.   */
    uint8_t digits[512];          /* ample for all tests */
    size_t len = 0;
    seq_t t = v;
    while (t != (seq_t)0) {
        digits[len++] = (uint8_t)(t % (seq_t)8);
        t = t / (seq_t)8;
    }

    /* Check for the two possible orders: 05 or 50. */
    for (size_t i = 1; i < len; ++i) {
        if ((digits[i-1] == 0 && digits[i] == 5) ||
            (digits[i-1] == 5 && digits[i] == 0))
            return true;
    }
    return false;
}

/* Cache of previously computed terms.      */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_cnt  = 0;
static seq_t  last_checked = (seq_t)0;

/* Grow the cache array to hold at least new_cap numbers. */
static void ensure_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t grow = cache_cap ? cache_cap * 2 : 16;
    while (grow < new_cap) grow <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, grow * sizeof(seq_t));
    cache = tmp;
    cache_cap = grow;
}

/* Sequence entry at index n (1‑based). */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n < 1 */

    /* Return cached value if already known. */
    if (cache_cnt >= (size_t)n) return cache[n-1];

    /* Search until we discover the needed number of terms. */
    idx_t target = n;
    seq_t cand = last_checked + (seq_t)1;

    while (cache_cnt < target) {
        if (has05(cand) && !has05(cand + (seq_t)1)) {
            ensure_cache(cache_cnt + 1);
            cache[cache_cnt++] = cand;
        }
        cand = cand + (seq_t)1;
    }

    last_checked = cand - (seq_t)1;
    return cache[n-1];
}

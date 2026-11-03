/*
 * A069695  Triangular numbers with internal digits 3.
 * https://oeis.org/A069695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = the nth triangular number T(k) = k*(k+1)/2 * whose
 *            internal decimal digits are all 3. * Numbers with fewer than
 *            three digits satisfy the rule. The * first term a(1) equals
 *            T(0)=0
 * Range:     u8 n <= 16, u16 n <= 18, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: full, pass, 19 terms through n = 19 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Check that all internal digits of x are 3.  Numbers with
   fewer than three digits are accepted automatically. */
static bool interior_all_threes(seq_t x)
{
    if (x == 0)
        return true;

    /* Temporary buffer holds the decimal digits (least significant first). */
    uint8_t buf[40];          /* enough for 128‑bit numbers. */
    size_t len = 0;
    seq_t tmp = x;
    while (tmp != 0) {
        buf[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* If fewer than 3 digits, the number is acceptable. */
    if (len < 3)
        return true;

    /* Check interior digits: indices 1 .. len-2. */
    for (size_t i = 1; i + 1 < len; ++i)
        if (buf[i] != 3)
            return false;

    return true;
}

/* Cached sequence of accepted triangular numbers. */
static seq_t *cache      = NULL;
static size_t cache_sz   = 0;
static size_t cache_cap  = 0;

static void cache_append(seq_t val)
{
    if (cache_sz >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *new_arr = mem_resize(seq_t, cache, new_cap);
        if (!new_arr)
            abort();   /* fatal allocation failure */
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_sz++] = val;
}

/* Generate triangular numbers incrementally.  cur_k holds the
   current k (starting at 0), and cur_t holds T(cur_k). */
static idx_t  cur_k = 0;
static seq_t cur_t = 0;

/* Produces the next triangular number after the current one. */
static void next_triangular(void)
{
    ++cur_k;                               /* move to next k */
    cur_t += (seq_t)cur_k;                 /* cur_t now = T(cur_k) */
}

/* Main entry point. */
seq_t a(idx_t n)
{
    /* Generate new elements until we have at least n cached. */
    while (cache_sz < (size_t)n) {
        if (interior_all_threes(cur_t))
            cache_append(cur_t);
        next_triangular();
    }
    return cache[(size_t)n - 1];
}

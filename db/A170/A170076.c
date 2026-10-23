/*
 * A170076  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A170076:  Number of reduced words of length n
 *  in the Coxeter group on 19 generators with relations
 *  S_i^2 = I and (S_i S_j)^37 = I.
 *
 *  The sequence satisfies the linear recurrence
 *
 *      a(0) = 1
 *      a(n) = 17 * (a(n-1)+…+a(n-36)) - 153 * a(n-37) + c(n)      (n ≥ 1)
 *
 *  where the correction term c(n) is
 *
 *          c(n) = 2          for 1 ≤ n ≤ 36
 *          c(n) = 1          for n = 0 or n = 37
 *          c(n) = 0          for n > 37
 *
 *  Initial values (first few):
 *
 *      a(0) = 1
 *      a(1) = 19
 *      a(2) = 342
 *      a(3) = 6156
 *      a(4) = 110808
 *
 *  The recurrence uses only integer arithmetic.  The implementation
 *  keeps a sliding window of the last 36 terms to evaluate a(n)
 *  in O(1) per term.  A static cache provides memoisation.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache     = NULL;   /* memoisation buffer          */
static size_t cache_cap = 0;      /* allocated capacity (elements) */
static idx_t  cache_len = 0;      /* number of elements filled   */
static seq_t  win_sum   = 0;      /* sum of the last 36 terms    */

static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) exit(1);
    cache = p;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* we need at least indices 0 .. n */
    size_t needed = (size_t)n + 1;
    /* for the recurrence start we also need up to index 37 */
    if (needed < 38) needed = 38;
    ensure_capacity(needed);

    /* initialise cache if empty */
    if (cache_len == 0)
    {
        seq_t sum_prefix = 0;            /* sum of a[0..k-1] at step k */
        for (idx_t k = 0; k < 37; ++k)
        {
            seq_t val;
            if (k == 0)
                val = (seq_t)1;
            else
                val = (seq_t)17 * sum_prefix + (seq_t)2;
            cache[k] = val;
            sum_prefix += val;
        }

        /* prepare sliding window sum of a[1..36] */
        seq_t sum36 = 0;
        for (idx_t k = 1; k <= 36; ++k) sum36 += cache[k];
        win_sum = sum36;

        cache_len = 37;   /* indices 0 … 36 stored */
    }

    /* extend cache up to n if required */
    if (cache_len <= n)
    {
        for (idx_t i = cache_len; i <= n; ++i)
        {
            seq_t term = (seq_t)17 * win_sum
                       - (seq_t)153 * cache[i - 37];
            if (i == 37) term += (seq_t)1;
            cache[i] = term;

            /* update window sum for next iteration:
               drop a[i-36] and add a[i]          */
            win_sum += term - cache[i - 36];
        }
        cache_len = n + 1;
    }

    return cache[n];
}

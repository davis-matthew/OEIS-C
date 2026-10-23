/*
 * A170681  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 46 *
 *            (a(n-1)+a(n-2)+…+a(n-48)) – 1081 * a(n-49) for n >= 49. For 0 <=
 *            n <= 48 the sequence is given by a(n) = P_n + 46 *
 *            (a(0)+a(1)+…+a(n-1)), where P_0 = 1 and P_n = 2 for 1 <= n <= 48
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;        /* array of computed terms  */
static size_t cache_cap = 0;       /* current allocated capacity    */
static size_t cache_sz  = 0;       /* number of terms stored (next index to fill)  */
static seq_t window_sum48 = 0;     /* sum of the last 48 computed terms              */
static seq_t total_sum   = 0;      /* sum of all computed terms up to the newest    */

/* ensure that cache can hold at least n+1 elements*/
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();    /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_capacity(n);

    if (n < cache_sz)          /* already computed */
        return cache[n];

    /* compute terms up to n */
    for (idx_t i = (idx_t)cache_sz; i <= n; ++i) {
        seq_t val;
        if (i <= 48) {
            /* P_i : 1 if i==0 else 2 */
            seq_t p = (i == 0 ? (seq_t)1 : (seq_t)2);
            val = p + (seq_t)46 * total_sum;
            /* store term */
            cache[i] = val;
            ++cache_sz;
            total_sum += val;           /* keep running total for i<=48 */
            /* update sliding sum of last 48 terms */
            window_sum48 += val;
            if (cache_sz > 48)          /* remove element that moved out of window */
                window_sum48 -= cache[cache_sz - 49];
        } else { /* i >= 49 */
            /* recurrence: a(i) = 46*window_sum48 - 1081*a(i-49)   */
            seq_t prev49 = cache[i - 49];
            val = (seq_t)46 * window_sum48 - (seq_t)1081 * prev49;
            cache[i] = val;
            ++cache_sz;
            /* update sliding sum for next iteration */
            window_sum48 += val;
            /* subtract the oldest term that leaves the window */
            size_t oldest_index = i - 48; /* oldest index in new window */
            if (oldest_index < cache_sz)
                window_sum48 -= cache[oldest_index];
        }
    }
    return cache[n];
}

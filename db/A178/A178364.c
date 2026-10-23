/*
 * A178364  Numbers with rounded up arithmetic mean of digits = 4.
 * https://oeis.org/A178364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A178364 – Numbers whose rounded‑up arithmetic mean of digits is 4
 *  A(n) is the n‑th positive integer N for which
 *      ceil( digit_sum(N) / digit_len(N) ) = 4
 *  Equivalently: 3 * len < sum <= 4 * len
 *  where len  = number of decimal digits of N
 *            sum  = sum of its decimal digits
 *
 *  The function builds the sequence lazily, caching all terms
 *  that have been generated so far.  It works for any seq_t /
 *  idx_t used by the build system.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache      = NULL;   /* cached sequence values      */
static idx_t  cache_cnt  = 0;      /* number of terms cached      */
static idx_t  cache_cap  = 0;      /* allocated capacity          */
static idx_t  next_candidate = 1;  /* next integer to test        */

/* ensure that the cache can hold at least 'need' terms   */
static void ensure_cache(idx_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        return;  /* out of memory – caller will receive wrong values   */
    for (idx_t i = 0; i < cache_cnt; ++i)
        new_arr[i] = cache[i];
    if (cache)
        free(cache);
    cache      = new_arr;
    cache_cap  = new_cap;
}

/* compute a(n) – 1‑based indexing */
seq_t a(idx_t n)
{
    if (n == 0)           /* the OEIS offset is 1; outside of bounds */
        return 0;

    /* generate terms until we have at least n of them */
    if (cache_cnt < n) {
        ensure_cache(n);
        while (cache_cnt < n) {
            /* compute digit sum and length of next_candidate   */
            idx_t tmp   = next_candidate;
            unsigned long long sum = 0;
            unsigned long long len = 0;
            if (tmp == 0) {           /* only for candidate = 0, never happens */
                sum = 0;
                len = 1;
            } else {
                while (tmp > 0) {
                    sum += tmp % 10;
                    len += 1;
                    tmp /= 10;
                }
            }
            /* condition: 3*len < sum <= 4*len */
            if ( (unsigned long long)3 * len < sum &&
                 sum <= (unsigned long long)4 * len )
            {
                cache[cache_cnt++] = (seq_t)next_candidate;
            }
            ++next_candidate;
        }
    }
    return cache[n - 1];
}

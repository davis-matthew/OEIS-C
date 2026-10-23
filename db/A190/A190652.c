/*
 * A190652  Years with exactly two "Friday the 13ths", starting from 1901.
 * https://oeis.org/A190652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) - the nth year after 1901 having exactly two Friday the
 *            13th dates
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static state for memoization */
static seq_t *cache = NULL;      /* array of cached years */
static size_t cache_len = 0;     /* number of valid terms in cache   */
static size_t cache_cap = 0;     /* current allocation capacity     */
static int next_year = 1901;     /* next year to test */

static void ensure_cache(idx_t n)
{
    while (cache_len < n)
    {
        int year   = next_year++;       /* year to test                */
        int count  = 0;                 /* number of Friday the 13th   */

        /* Sakamoto's algorithm for Gregorian calendar:
           w = 0 -> Sunday, 1 -> Monday, ..., 6 -> Saturday   */
        static const int t[12] = {0,3,2,5,0,3,5,1,4,6,2,4};

        for (int month = 1; month <= 12; ++month)
        {
            long long y = year;
            if (month < 3) y--;                 /* March = 3, … */
            long long w = (y + y/4 - y/100 + y/400 +
                         t[month-1] + 13) % 7;    /* 13th of month          */
            if (w == 5) count++;                /* Friday (0=Sun, 5=Fri)  */
        }

        if (count == 2)
        {
            /* grow the cache if necessary */
            if (cache_len + 1 > cache_cap)
            {
                size_t newcap = cache_cap ? cache_cap * 2 : 32;
                cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                cache_cap = newcap;
            }
            cache[cache_len++] = (seq_t)year;   /* store the year          */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for zero, return 0 */
    ensure_cache(n);                /* make sure we have n terms   */
    return cache[n - 1];            /* a(1) is at index 0          */
}

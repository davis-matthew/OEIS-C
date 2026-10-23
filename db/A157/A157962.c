/*
 * A157962  Table read by rows: Occurrences of Friday the 13th, T(n,1)=month
 *          (1 for January), T(n,2)=year-2000, starting year 2000.
 * https://oeis.org/A157962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   sequence A157962 – table of month and year‑2000 for each
 *            Friday‑13th. The function keeps an internal cache of the
 *            computed values and extends it on demand. It works for any seq_t
 *            type that supports simple assignment and can be allocated with
 *            alloc_seq_array / free_seq_array
 * Range:     u8 n <= 881, u16 n <= 3442, u32 n <= 3442, u64 n <= 3442,
 *            u128 n <= 3442, big any n
 * Helpers:   alloc_seq_array, days_in_month, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/days_in_month.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* sequence undefined for n < 1 */
        return (seq_t)0;

    /* ---------- static state --------------------------------------------------- */
    static seq_t *cache = NULL;        /* pointer to the cached array      */
    static size_t cache_sz = 0;        /* number of terms stored           */
    static size_t cache_cap = 0;       /* allocated capacity               */

    static int year  = 2000;   /* current year we are processing       */
    static int month = 1;      /* current month (1‑12)                  */
    static int dow   = 6;      /* weekday of the 1st day of the month
                                 0=Sunday … 6=Saturday, Jan 1 2000 was Saturday */

    /* Ensure the cache can hold at least n terms (plus a small safety margin). */
    if (cache_cap < (size_t)n)
    {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 256;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *new_arr = alloc_seq_array(new_cap);
        for (size_t i = 0; i < cache_sz; ++i)
            new_arr[i] = cache[i];
        if (cache)
            free_seq_array(cache);
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_sz < (size_t)n)
    {
        /* Friday‑13th occurs if the 13th falls on a Friday. */
        if (((dow + 12) % 7) == 5)
        {
            /* Ensure capacity for the two new values. */
            if (cache_sz + 2 > cache_cap)
            {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 256;
                if (new_cap < cache_sz + 2)
                    new_cap = cache_sz + 2;
                seq_t *new_arr = alloc_seq_array(new_cap);
                for (size_t i = 0; i < cache_sz; ++i)
                    new_arr[i] = cache[i];
                if (cache)
                    free_seq_array(cache);
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)month;
            cache[cache_sz++] = (seq_t)(year - 2000);
        }

        /* Advance to the next month. */
        int dim = days_in_month(year, month);
        dow = (dow + dim) % 7;
        month++;
        if (month == 13)
        {
            month = 1;
            year++;
        }
    }

    return cache[n - 1];
}

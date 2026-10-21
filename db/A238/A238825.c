/*
 * A238825  a(1)..a(4) = 0,0,0,1; thereafter a(n) =
 *          a(n-2)+a(n-3)+2*(d(n-3)+d(n-4)) where d(n) = A238824(n).
 * https://oeis.org/A238825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   The recurrence for A238825 is a(n) = a(n-2) + a(n-3) + 2 * (
 *            d(n-3) + d(n-4) ), where d(n) = A238824(n). Eliminating d gives,
 *            for n ≥ 13, a(n) = 3*a(n-1) - 4*a(n-3) + a(n-4) + a(n-5) +
 *            3*a(n-6) - a(n-7). Initial values (n = 1 … 12): a(1)=0, a(2)=0,
 *            a(3)=0, a(4)=1, a(5)=2, a(6)=5, a(7)=11, a(8)=27...
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 53, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity (including unused index 0) */
    static idx_t  len   = 0;   /* actual number of computed terms (len-1 == max n stored) */

    /* base values up to n = 12 */
    static const seq_t base[13] = {
        0,          /* index 0 unused */
        0,          /* a(1) */
        0,          /* a(2) */
        0,          /* a(3) */
        1,          /* a(4) */
        2,          /* a(5) */
        5,          /* a(6) */
        11,         /* a(7) */
        27,         /* a(8) */
        64,         /* a(9) */
        158,        /* a(10) */
        387,        /* a(11) */
        956         /* a(12) */
    };

    /* initialise cache if needed */
    if (!cache) {
        /* allocate enough space for indices 0 … 12  */
        size_t needed = 13;      /* includes unused index 0 */
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) abort();
        cap = needed;
        len = 1;                 /* we will fill indices from 1 upwards */
    }

    /* ensure cache holds up to index n */
    if (n + 1 > cap)  {
        size_t new_cap = cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();
        cap = new_cap;
    }

    /* if not yet computed, fill from current len up to n inclusive */
    for (idx_t i = len; i <= n; ++i) {
        if (i <= 12) {
            cache[i] = base[i];
        } else {
            /* recurrence with only additions and one subtraction */
            seq_t pos = cache[i-1] * 3
                        + cache[i-4]
                        + cache[i-5]
                        + cache[i-6] * 3;
            seq_t neg = cache[i-3] * 4
                        + cache[i-7];
            cache[i] = pos - neg;
        }
    }
    len = n + 1;   /* all indices up to n are now valid */

    return cache[n];
}

/*
 * A188270  Number of nondecreasing strings of numbers (x(i), i=1..n) in -2..2
 *          with sum x(i)^3 equal to 0.
 * https://oeis.org/A188270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) + a(n-9) - 2*a(n-10) +
 *            2*a(n-12) - a(n-13) with initial values a(1)=1, a(2)=3, a(3)=3,
 *            a(4)=6, a(5)=6, a(6)=10, a(7)=10, a(8)=15, a(9)=17, a(10)=23,
 *            a(11)=27, a(12)=34, a(13)=40 (values for n < 1 are undefined,
 *            a(0) is returned as 0 for safety)
 * Range:     u8 n <= 27, u16 n <= 189, u32 n <= 7738, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* cached values */
    static size_t cap   = 0;           /* allocated capacity */
    static size_t sz    = 0;           /* current number of cached terms */

    if (n == 0) return 0;              /* safety against out‑of‑range */

    /* one‑time allocation and initialization of the first 13 terms */
    if (cache == NULL) {
        cap = 16;                      /* start with a modest capacity */
        sz  = 13;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – return 0 */

        static const seq_t init[13] = {
            1, 3, 3, 6, 6, 10, 10, 15, 17, 23, 27, 34, 40
        };
        for (size_t i = 0; i < 13; ++i) cache[i] = init[i];
    }

    /* expand the array if needed */
    if ((size_t)n > sz) {
        if (cap < (size_t)n) {
            size_t newcap = (size_t)n * 2;
            if (newcap < (size_t)n) newcap = (size_t)n;  /* overflow guard */
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return 0;      /* allocation failure – return 0 */
            cache = tmp;
            cap = newcap;
        }

        /* compute values from sz (currently cached) up to n-1 (zero‑based) */
        for (size_t idx = sz; idx < (size_t)n; ++idx) {
            /* use signed long long to avoid unsigned underflow in the
               recurrence until the final assignment. */
            long long v = 0;
            v +=  2LL * (long long)cache[idx - 1];
            v -=  2LL * (long long)cache[idx - 3];
            v +=  1LL * (long long)cache[idx - 4];
            v +=  1LL * (long long)cache[idx - 9];
            v -=  2LL * (long long)cache[idx -10];
            v +=  2LL * (long long)cache[idx -12];
            v -=  1LL * (long long)cache[idx -13];
            cache[idx] = (seq_t)v;
        }
        sz = (size_t)n;
    }

    return cache[n-1];          /* zero‑based indexing */
}

/*
 * A224574  Number of (n+5) X 10 0..1 matrices with each 6 X 6 subblock
 *          idempotent.
 * https://oeis.org/A224574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 16, u32 n <= 60, u64 n <= 149, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A224574
   Recurrence (for n > 16):
     a(n) = 4*a(n-1) - 6*a(n-2) + 3*a(n-3) + 3*a(n-4) - 6*a(n-5) + 5*a(n-6)
            - 4*a(n-7) + 3*a(n-8) - 3*a(n-10) + 3*a(n-11) - a(n-12)
   Initial values (1‑based):
     a(1)=22096, a(2)=7034,  a(3)=7295,  a(4)=8143,
     a(5)=8880,  a(6)=9480,  a(7)=9946,  a(8)=11672,
     a(9)=14594, a(10)=18457, a(11)=22982, a(12)=27995,
     a(13)=33345, a(14)=40295, a(15)=50032, a(16)=63512
*/

#include <stddef.h>  /* idx_t is defined elsewhere */
#include <stdlib.h>  /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* ---- Initial values ---- */
    static const seq_t init[17] = {
        0ULL,       /* dummy value for index 0 */
        22096ULL, 7034ULL,  7295ULL,  8143ULL,
        8880ULL,   9480ULL,  9946ULL, 11672ULL,
        14594ULL,  18457ULL, 22982ULL, 27995ULL,
        33345ULL,  40295ULL, 50032ULL, 63512ULL
    };

    /* ---- Memoisation cache (1‑based) ---- */
    static seq_t *cache = NULL;    /* array of computed values */
    static size_t cap   = 0;       /* allocated capacity (>= n+1) */
    static size_t used  = 0;       /* highest index already computed */

    /* ---- Grow the cache if necessary ---- */
    if ((size_t)n > cap) {
        size_t newcap = cap ? cap * 2 : 32;
        while (newcap < (size_t)n + 1) newcap <<= 1;
        cache = (seq_t *)realloc(cache, (newcap + 1) * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure – caller may handle */
        /* initialise new slots to 0 (optional for unsigned types) */
        for (size_t i = cap + 1; i <= newcap; ++i) cache[i] = 0;
        cap = newcap;
    }

    /* ---- Return already computed value ---- */
    if ((size_t)n <= used) return cache[n];

    /* ---- Compute terms up to n ---- */
    for (size_t i = used + 1; i <= (size_t)n; ++i) {
        if (i <= 16) {
            cache[i] = init[i];
        } else {
            seq_t val = 0;
            val += 4 * cache[i-1];
            val -= 6 * cache[i-2];
            val += 3 * cache[i-3];
            val += 3 * cache[i-4];
            val -= 6 * cache[i-5];
            val += 5 * cache[i-6];
            val -= 4 * cache[i-7];
            val += 3 * cache[i-8];
            val -= 3 * cache[i-10];
            val += 3 * cache[i-11];
            val -= 1 * cache[i-12];
            cache[i] = val;
        }
    }
    used = (size_t)n;
    return cache[n];
}

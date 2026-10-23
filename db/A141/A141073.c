/*
 * A141073  List of central integer pairs in Pascal-like triangles with index
 *          of asymmetry y = 3 and index of obliqueness z = 0 or z = 1.
 * https://oeis.org/A141073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A141073:   central integer pairs in Pascal‑like triangles,  
 * recurrence: a(1)=1, a(2)=1, a(3)=4, a(4)=2, a(5)=8, 
 * a(6)=4, a(7)=17, a(8)=8, a(9)=35, 
 * a(n) = a(n-2)+a(n-4)+2*a(n-6)+a(n-8)  for n >= 10
 *
 *   (This follows from the generating function in the OEIS entry.)
 *   Memoisation is used so the function can be called in any order.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed terms, 0‑based */
static size_t cache_cap = 0;          /* number of terms stored in cache */

/* external linkage as required */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined, but guard */
        return 0;

    /* Grow the cache if needed (C or C++). */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        /* Compute terms that are not yet cached. */
        for (idx_t i = cache_cap + 1; i <= n; ++i) {
            seq_t val;
            switch ((idx_t)i) {
                case 1: val = 1;                  break;
                case 2: val = 1;                  break;
                case 3: val = 4;                  break;
                case 4: val = 2;                  break;
                case 5: val = 8;                  break;
                case 6: val = 4;                  break;
                case 7: val = 17;                 break;
                case 8: val = 8;                  break;
                case 9: val = 35;                 break;
                default:
                    /* recurrence: a(n)=a(n-2)+a(n-4)+2*a(n-6)+a(n-8)   */
                    val = cache[(size_t)(i-3)] +      /* a(i-2) */
                          cache[(size_t)(i-5)] +      /* a(i-4) */
                          (seq_t)2 * cache[(size_t)(i-7)] + /* 2*a(i-6) */
                          cache[(size_t)(i-9)];       /* a(i-8) */
                    break;
            }
            cache[(size_t)(i-1)] = val;
        }
        cache_cap = new_cap;
    }

    return cache[(size_t)(n-1)];
}

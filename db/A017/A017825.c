/*
 * A017825  Expansion of 1/(1-x^3-x^4-x^5-x^6-x^7-x^8-x^9-x^10-x^11-x^12).
 * https://oeis.org/A017825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 62, u64 n <= 121,
 *            u128 n <= 239, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = a(n−3)+a(n−4)+…+a(n−12) for n>12
   with initial values: a(0)=1, a(1)=0, a(2)=0,
   a(3)=1, a(4)=1, a(5)=1, a(6)=2, a(7)=3, a(8)=4,
   a(9)=6, a(10)=9, a(11)=13, a(12)=19
   (OEIS A017825) */
#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values   */
static idx_t cache_size = 0;     /* number of cached entries */

/* Return a(n) for any n >= 0, using memoisation. */
seq_t a(idx_t n)
{
    /* expand cache if necessary */
    if (n + (idx_t)1 > cache_size) {
        size_t new_len = (size_t)(n + (idx_t)1);   /* new number of entries needed */
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) { /* allocation failed; return 0 as fallback */
            return (seq_t)0;
        }
        cache = new_cache;

        /* initialise the freshly allocated entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* handle the exact initial values */
            if (i == 0)          cache[i] = (seq_t)1;
            else if (i <= 2)     cache[i] = (seq_t)0;
            else if (i == 3)     cache[i] = (seq_t)1;
            else if (i == 4)     cache[i] = (seq_t)1;
            else if (i == 5)     cache[i] = (seq_t)1;
            else if (i == 6)     cache[i] = (seq_t)2;
            else if (i == 7)     cache[i] = (seq_t)3;
            else if (i == 8)     cache[i] = (seq_t)4;
            else if (i == 9)     cache[i] = (seq_t)6;
            else if (i == 10)    cache[i] = (seq_t)9;
            else if (i == 11)    cache[i] = (seq_t)13;
            else if (i == 12)    cache[i] = (seq_t)19;
            else { /* recurrence for i >= 13 */
                seq_t sum = 0;
                idx_t k;
                for (k = 3; k <= 12; ++k) {
                    sum += cache[i - k];
                }
                cache[i] = sum;
            }
        }
        cache_size = new_len;
    }
    return cache[n];
}

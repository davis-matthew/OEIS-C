/*
 * A154154  Numbers k such that 30 plus the k-th triangular number is a
 *          perfect square.
 * https://oeis.org/A154154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence A154154: numbers k such that 30 + k(k+1)/2 is a perfect square.
   Recurrence: a(n) = a(n-1)+6*a(n-2)-6*a(n-3)-a(n-4)+a(n-5)
   Initial terms: a(1)=3, a(2)=13, a(3)=34, a(4)=84, a(5)=203. */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset is 1 */

    /* lazily grow the cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if ((size_t)n > cache_len) {
        /* ensure capacity */
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        if (new_cap != cache_cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* initialise missing entries */
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            seq_t val;
            if (i == 0)           val = 3;   /* a(1) */
            else if (i == 1)      val = 13;  /* a(2) */
            else if (i == 2)      val = 34;  /* a(3) */
            else if (i == 3)      val = 84;  /* a(4) */
            else if (i == 4)      val = 203; /* a(5) */
            else {
                val = cache[i-1];
                val += 6 * cache[i-2];
                val -= 6 * cache[i-3];
                val -= cache[i-4];
                val += cache[i-5];
            }
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n-1];
}

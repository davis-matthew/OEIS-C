/*
 * A209386  a(n) = 2*A005132(n).
 * https://oeis.org/A209386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n>0, a(n)=a(n-1)-2n if a(n-1)>=2n and that value
 *            hasn't appeared before, otherwise a(n)=a(n-1)+2n. This is
 *            2*A005132(n)
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;        /* array of computed terms   */
    static size_t cap = 0;             /* current allocated size    */
    static idx_t len = 0;              /* number of terms stored    */

    /* Initialise cache with a(0)=0 */
    if (len == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();          /* out of memory            */
        cache[0] = 0;
        len = 1;
    }

    /* Grow cache if needed */
    if (len <= n) {
        while (len <= n) {
            idx_t k = len;                    /* next index to compute   */
            seq_t prev = cache[len - 1];      /* a(k-1)                  */
            seq_t step = (seq_t)k * (seq_t)2; /* 2k                      */
            seq_t next;

            /* If subtracting does not underflow, try it */
            if (prev >= step) {
                seq_t cand = prev - step;

                /* Check that cand hasn't appeared before */
                bool found = false;
                for (idx_t i = 0; i < len; ++i) {
                    if (cache[i] == cand) {
                        found = true;
                        break;
                    }
                }

                next = found ? prev + step : cand;
            } else {
                /* Subtraction would underflow; must add */
                next = prev + step;
            }

            /* Ensure capacity */
            if (len == (idx_t)cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* out of memory            */
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = next;
        }
    }

    return cache[n];
}

/*
 * A037662  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,0.
 * https://oeis.org/A037662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3; for n>1, a(n)=a(n‑1)*6 + d((n‑1) mod 3)
   where d[0]=3, d[1]=1, d[2]=0.  This builds the number whose
   base‑6 representation consists of the first n digits of the
   repeating sequence 3,1,0.  The function memoises values to
   satisfy arbitrary call order. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Guard against the possibly ill‑defined case n==0
       (the sequence is defined only for n>=1). */
    if (n == 0) return 0;

    /* Local cache of computed terms */
    static seq_t *cache = NULL;      /* a(1) .. a(cache_len) */
    static size_t cache_len = 0;     /* number of terms currently cached */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* Return cached value if already computed */
    if ((size_t)n <= cache_len) {
        return cache[n - 1];
    }

    /* Grow cache to at least n elements if necessary */
    size_t needed = (size_t)n;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {  /* allocation failed */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from current cache_len up to n-1 */
    for (idx_t i = cache_len; i < n; ++i) {
        int mod = (int)(i % 3);              /* pattern index 0,1,2 */
        seq_t digit = (mod == 0 ? (seq_t)3
                        : (mod == 1 ? (seq_t)1 : (seq_t)0));
        if (i == 0) {                         /* a(1) */
            cache[0] = digit;
        } else {
            cache[i] = cache[i - 1] * (seq_t)6 + digit;
        }
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}

/*
 * A117713  a(1)=1, a(2)=3, a(3)=8; for n>=4, a(n) = 10*a(n-3) + 8 (if a(n-3)
 *          is odd) or + 9 (if a(n-3) is even).
 * https://oeis.org/A117713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117713: a(1)=1, a(2)=3, a(3)=8;
   for n>=4: a(n) = 10*a(n-3)+8 if a(n-3) is odd, else +9. */
/* The function memoizes results so that calls with arbitrary n in any order
   return the correct term. */

#include <stdlib.h>   /* malloc, realloc, abort */

static void ensure_capacity(size_t needed, size_t *cap, seq_t **p)
{
    if (needed <= *cap) return;            /* already big enough */
    size_t new_cap = (*cap) ? (*cap << 1) : 1;
    while (new_cap < needed) new_cap <<= 1; /* grow geometrically */
    seq_t *tmp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
    if (!tmp) abort();                     /* terminate on allocation failure */
    *p   = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;   /* number of terms currently computed */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Ensure the cache can hold at least 'n' terms (indices 0..n-1). */
    ensure_capacity((size_t)n, &cache_cap, &cache);

    /* Compute any missing terms. */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        if (i == 0) {                      /* a(1) */
            cache[0] = (seq_t)1;
        } else if (i == 1) {                /* a(2) */
            cache[1] = (seq_t)3;
        } else if (i == 2) {                /* a(3) */
            cache[2] = (seq_t)8;
        } else {
            seq_t prev = cache[i - 3];      /* a(i-3+1) = a(i-2) */
            if ((prev & (seq_t)1) == 0) {   /* prev is even */
                cache[i] = prev * (seq_t)10 + (seq_t)9;
            } else {                       /* prev is odd */
                cache[i] = prev * (seq_t)10 + (seq_t)8;
            }
        }
    }
    cache_len = (size_t)n;

    return cache[n - 1];
}

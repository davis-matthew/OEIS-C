/*
 * A125501  The (1,1)-entry in the matrix M^n, where M is the 7 X 7 Cartan
 *          matrix [2,-1,0,0,0,0,0; -1,2,-1,0,0,0,0; 0,-1,2,-1,0,0,-1;
 *          0,0,-1,2,-1,0,0; 0,0,0,-1,2,-1,0; 0,0,0,0,-1,2,0; 0,0,-1,0,0,0,2].
 * https://oeis.org/A125501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function:
   a(n) = 12*a(n-1) - 54*a(n-2) + 112*a(n-3) - 105*a(n-4)
          + 36*a(n-5) - a(n-6)
   with initial values a(0)=1, a(1)=2, a(2)=5, a(3)=14,
   a(4)=42, a(5)=132. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated size */
    static size_t size = 0;  /* number of terms currently stored */

    /* first-time allocation and initialization of the first six terms */
    if (cap == 0) {
        cap = 16;                       /* generous initial capacity */
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure, should not happen */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)42;
        cache[5] = (seq_t)132;
        size   = 6;                     /* we have stored terms 0..5 */
    }

    /* if the requested term is already known, return it */
    if (n < size)
        return cache[n];

    /* grow the allocated buffer if necessary */
    if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;   /* double until it fits */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        /* recurrence requires terms i-1 .. i-6 */
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];

        seq_t val = 12 * a1;
        val      -= 54 * a2;
        val      += 112 * a3;
        val      -= 105 * a4;
        val      += 36 * a5;
        val      -= a6;

        cache[i] = val;
    }

    size = n + 1;   /* we now have terms up to and including n */
    return cache[n];
}

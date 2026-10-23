/*
 * A189429  Number of n X 3 array permutations with each element not moving,
 *          or moving one space N, SW or SE.
 * https://oeis.org/A189429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + 10*a(n-3) + 12*a(n-4) + 8*a(n-5) with
 *            a(1)=1, a(2)=3, a(3)=9, a(4)=31, a(5)=109
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoises previously computed values in a static array.
   It works for all builds where seq_t is an arbitrary-precision integer
   class (C++), a built‑in unsigned integer type (C 11), or any other
   integral type.  The array is resized with realloc/calloc, and a C
   style cast is used so the code compiles in both C and C++. */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    if (n < 1)                 /* the sequence is defined for n>=1   */
        return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;   /* largest index computed so far  */
    static idx_t cache_cap   = 0;   /* allocated array capacity       */

    /* enlarge cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
        if (new_cap <= n)
            new_cap = n + 1;        /* ensure space for index n       */

        /* realloc may return NULL only on allocation failure */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* unrecoverable allocation error */

        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to index n, if not already done */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        switch (i) {
            case 1: cache[i] = (seq_t)1; break;
            case 2: cache[i] = (seq_t)3; break;
            case 3: cache[i] = (seq_t)9; break;
            case 4: cache[i] = (seq_t)31; break;
            case 5: cache[i] = (seq_t)109; break;
            default:
                /* recurrence: a(n) = a(n-1)+4*a(n-2)+10*a(n-3)+12*a(n-4)+8*a(n-5) */
                cache[i] =
                    cache[i-1]
                  + (seq_t)4 * cache[i-2]
                  + (seq_t)10 * cache[i-3]
                  + (seq_t)12 * cache[i-4]
                  + (seq_t)8  * cache[i-5];
                break;
        }
    }
    cache_len = n;

    return cache[(size_t)n];
}

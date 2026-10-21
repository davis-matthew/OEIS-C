/*
 * A200840  Number of 0..n arrays x(0..4) of 5 elements without any two
 *          consecutive increases or two consecutive decreases.
 * https://oeis.org/A200840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 107, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A200840
   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) - a(n-6)
   Initial values: a(1)=32, a(2)=191, a(3)=676, a(4)=1817,
                    a(5)=4108, a(6)=8239
   Implementation uses an expanding cache so calls in any order run
   in O(max(n)) time.  The recurrence is evaluated using seq_t
   arithmetic, which automatically wraps for small types or keeps
   exact values for arbitrary‑precision types. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* out of bounds – never used */

    /* static cache: a[0] = a(1), a[1] = a(2), … */
    static seq_t *a_cache      = NULL;
    static size_t cache_len   = 0;     /* number of terms stored            */
    static size_t cache_cap   = 0;     /* allocated capacity                 */

    /* enlarge cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {              /* allocation failed – fall back to
                                            pure recurrence calculation */
            a_cache = NULL;
            cache_len = 0;
            cache_cap = 0;
            /* compute only the requested term, using the recurrence
               with seq_t arithmetic.  We treat the first 6 terms
               specially. */
            if (n <= 6) {
                static const seq_t init[6] = {
                    (seq_t)32,  (seq_t)191, (seq_t)676,
                    (seq_t)1817,(seq_t)4108,(seq_t)8239
                };
                return init[n-1];
            }
            /* compute recursively – depth limited by n (≤10000) */
            seq_t a1 = a(n-1);
            seq_t a2 = a(n-2);
            seq_t a3 = a(n-3);
            seq_t a4 = a(n-4);
            seq_t a5 = a(n-5);
            seq_t a6 = a(n-6);
            return  (seq_t)( (seq_t)6 * a1
                           - (seq_t)15 * a2
                           + (seq_t)20 * a3
                           - (seq_t)15 * a4
                           + (seq_t)6  * a5
                           - a6 );
        }
        a_cache = tmp;
        cache_cap = new_cap;
    }

    /* fill cache up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        if (i < 6) {                   /* initial values (i: 0..5 correspond to n=1..6) */
            static const seq_t init[6] = {
                (seq_t)32,  (seq_t)191, (seq_t)676,
                (seq_t)1817,(seq_t)4108,(seq_t)8239
            };
            a_cache[i] = init[i];
        } else {                       /* recurrence */
            a_cache[i] =  (seq_t)( (seq_t)6 * a_cache[i-1]
                                 - (seq_t)15 * a_cache[i-2]
                                 + (seq_t)20 * a_cache[i-3]
                                 - (seq_t)15 * a_cache[i-4]
                                 + (seq_t)6  * a_cache[i-5]
                                 - a_cache[i-6] );
        }
    }
    cache_len = (size_t)n;

    return a_cache[n-1];
}

/*
 * A178885  Partial sums of Berstel sequence (A007420).
 * https://oeis.org/A178885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-2) + 8*a(n-3) - 4*a(n-4) Initial values:
 *            a(0)=0, a(1)=0, a(2)=1, a(3)=3
 * Range:     i8 n <= 11, i16 n <= 21, i32 n <= 41, i64 n <= 80, i128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised sequence values */
static idx_t  capacity = 0;   /* allocated size (in elements)      */
static idx_t  computed = 0;   /* number of elements already filled */

/* Ensure the cache contains at least `need` elements and the first four
   values are initialized. */
static void ensure_capacity(idx_t need)
{
    idx_t min_cap = need > 4 ? need : 4;     /* at least four slots */
    if (capacity < min_cap) {
        /* reallocate */
        seq_t *tmp = (seq_t *)realloc(cache, min_cap * sizeof(seq_t));
        if (!tmp) {
            /* out of memory – abort (unreachable in the test harness) */
            exit(1);
        }
        cache = tmp;
        capacity = min_cap;
    }

    /* initialise the first four values if not already done */
    if (computed < 4) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        computed = 4;
    }
}

seq_t a(idx_t n)
{
    if (n < 0) return (seq_t)0;       /* safety guard, not required by spec */

    /* make sure we can store up to index n */
    ensure_capacity(n + 1);

    /* compute missing terms using the recurrence */
    for (idx_t i = computed; i <= n; ++i) {
        /* recurrence is valid for i >= 4 */
        cache[i] =  (seq_t)3 * cache[i-1]
                  - (seq_t)6 * cache[i-2]
                  + (seq_t)8 * cache[i-3]
                  - (seq_t)4 * cache[i-4];
    }
    computed = n + 1;    /* all indices up to n are now filled */

    return cache[n];
}

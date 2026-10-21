/*
 * A221572  Number of 0..7 arrays of length n with each element differing from
 *          at least one neighbor by something other than 1.
 * https://oeis.org/A221572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; a(2)=50; a(3)=314; a(4)=2500; a(5)=19014; a(6)=145800;
 *            a(7)=1116400; a(8)=8550512;
 *            a(n)=7*a(n-1)+4*a(n-2)+5*a(n-3)+20*a(n-4)+20*a(n-5)+23*a(n-6)-6*a(n-7)+3*a(n-8)
 *            for n>=9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed values, indexes correspond to n directly */
    static seq_t *cache   = NULL;
    static size_t  heap   = 0;   /* capacity (indexes 0..heap-1)   */
    static size_t  seen   = 0;   /* largest n that has been computed */

    /* initial values for n = 1 .. 8 */
    static const seq_t init[8] = {
        (seq_t)0,
        (seq_t)50,
        (seq_t)314,
        (seq_t)2500,
        (seq_t)19014,
        (seq_t)145800,
        (seq_t)1116400,
        (seq_t)8550512
    };

    /* a(0) is undefined in the OEIS entry; return 0 for safety */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Ensure the cache has room for index n.  Capacity = n+1 (index 0 unused).  */
    if ((size_t)n + 1 > heap) {
        size_t newcap = (size_t)n + 1;
        /* Reallocate memory (explicit cast works both in C and C++) */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failure – undefined behaviour, but return 0 */
            return (seq_t)0;
        }
        cache = tmp;
        heap  = newcap;
        /* initialize the new slots to zero; they will be overwritten anyway */
        for (size_t i = seen + 1; i < heap; ++i) {
            cache[i] = (seq_t)0;
        }
    }

    /* Compute all missing values up to n. */
    for (size_t i = seen + 1; i <= (size_t)n; ++i) {
        seq_t val;
        if (i <= 8) {                    /* use known initial terms */
            val = init[(size_t)i - 1];
        } else {
            /* recurrence: a(i) = 7*a(i-1)+4*a(i-2)+5*a(i-3)+20*a(i-4)+20*a(i-5)+23*a(i-6)-6*a(i-7)+3*a(i-8) */
            val = (seq_t)7  * cache[i - 1] +
                  (seq_t)4  * cache[i - 2] +
                  (seq_t)5  * cache[i - 3] +
                  (seq_t)20 * cache[i - 4] +
                  (seq_t)20 * cache[i - 5] +
                  (seq_t)23 * cache[i - 6] -
                  (seq_t)6  * cache[i - 7] +
                  (seq_t)3  * cache[i - 8];
        }
        cache[i] = val;
    }

    seen = (size_t)n;
    return cache[n];
}

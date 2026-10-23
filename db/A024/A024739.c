/*
 * A024739  a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), with
 *          a(1)=a(3)=2 and a(2)=1.
 * https://oeis.org/A024739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), a(1)=2,
 *            a(2)=1, a(3)=2; a(n) defined for n >= 1
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoises results in a dynamically grown array. */
#include <stdlib.h>

/* External linkage is required.  seq_t and idx_t are typedefed by the build. */
seq_t a(idx_t n)
{
    /* Static cache for computed terms and information about its size. */
    static seq_t *cache = NULL;
    static idx_t   allocated = 0;          /* number of cache entries allocated   */
    static idx_t   max_computed = 0;       /* largest n for which cache[n] is defined */

    /* Grow the cache if needed.  Allocate a bit more space than strictly
       required to reduce the number of reallocations. */
    if (n >= allocated) {
        idx_t new_cap = (n + 1) * 2;            /* double size to amortise reallocs */
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure: return 0 or abort */
            return (seq_t)0;
        }
        /* Copy existing values if any.  This copy is cheap since we
           already hold values in the old array. */
        if (cache) {
            for (idx_t i = 0; i < allocated; ++i) {
                tmp[i] = cache[i];
            }
            free(cache);
        }
        cache = tmp;
        allocated = new_cap;
    }

    /* Compute any missing terms up to n.  This loop is executed only
       once per n (increasing n across multiple calls).  The recurrence
       uses only earlier terms, so the indices are valid. */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        if (k == 1 || k == 3) {
            /* Initial values. */
            cache[k] = (seq_t)2;
        } else if (k == 2) {
            cache[k] = (seq_t)1;
        } else {
            /* Main recurrence. */
            seq_t sum = 0;
            idx_t limit = (k + 1) / 4;   /* floor((k+1)/4) */
            for (idx_t i = 1; i <= limit; ++i) {
                idx_t idx1 = 2 * i - 1;
                idx_t idx2 = k - 2 * i + 1;
                /* Multiply and add using seq_t arithmetic. */
                sum += cache[idx1] * cache[idx2];
            }
            cache[k] = sum;
        }
    }

    max_computed = (n > max_computed) ? n : max_computed;
    return cache[n];
}

/*
 * A081831  a(1)=a(2)=1, a(n) = a(a(n - a(n-1))) + a(n - a(n-2)).
 * https://oeis.org/A081831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 826, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A081831: a(1)=a(2)=1,  a(n)=a(a(n-a(n-1))) + a(n-a(n-2)). */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* cache[0] is unused; cache[1]..cache[last_computed] are valid */
    static seq_t *cache = NULL;
    static idx_t   cache_size   = 0;   /* allocated slots (incl. index 0) */
    static idx_t   last_computed = 0;   /* highest n that has been computed */

    if (n == (idx_t)0)                       /* undefined, but return 0 safely */
        return (seq_t)0;

    /* Resize cache if needed */
    if (n + (idx_t)1 > cache_size) {
        idx_t new_size = n + (idx_t)1;
        if (new_size < (idx_t)3) new_size = (idx_t)3;   /* at least for indices 1 and 2 */
        void *tmp = realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure */
        cache      = (seq_t *)tmp;
        cache_size = new_size;
    }

    /* Initialise the base cases (if not already done) */
    if (last_computed < (idx_t)1) {
        cache[1] = (seq_t)1;
        last_computed = (idx_t)1;
    }
    if (last_computed < (idx_t)2) {
        cache[2] = (seq_t)1;
        last_computed = (idx_t)2;
    }

    /* Compute any missing terms up to n */
    idx_t i_start = last_computed + (idx_t)1;
    if (i_start < (idx_t)3) i_start = (idx_t)3;   /* recurrence is defined for n >= 3 */

    for (idx_t i = i_start; i <= n; ++i) {
        seq_t val_nm1 = cache[i-(idx_t)1];        /* a(i-1) */
        seq_t val_nm2 = cache[i-(idx_t)2];        /* a(i-2) */

        /* indices needed by the recurrence */
        idx_t idx1 = i - (idx_t)val_nm1;          /* n - a(n-1)   */
        idx_t idx2 = i - (idx_t)val_nm2;          /* n - a(n-2)   */

        /* first term is a( a(n-a(n-1)) ) */
        idx_t inner = (idx_t)cache[idx1];         /* a(idx1)      */
        seq_t first_term  = cache[inner];

        seq_t second_term = cache[idx2];

        cache[i] = first_term + second_term;
    }

    if (n > last_computed)
        last_computed = n;

    return cache[n];
}

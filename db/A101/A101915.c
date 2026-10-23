/*
 * A101915  G.f. satisfies: A(x) = 1/(1 + x*A(x^5)) and also the continued
 *          fraction: 1+x*A(x^6) =
 *          [1;1/x,1/x^5,1/x^25,1/x^125,...,1/x^(5^(n-1)),...].
 * https://oeis.org/A101915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 20, i16 n <= 40, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101915
   Formula: a(0) = 1;
   a(n) = -∑_{k=0..⌊(n-1)/5⌋} a(k) * a(n-5*k-1)   for n ≥ 1
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *memo = NULL;                     /* array of computed terms      */
    static unsigned char *computed = NULL;         /* flags : 1 = value present    */
    static idx_t allocated = 0;                    /* current capacity of arrays   */
    static idx_t computed_len = 0;                 /* number of contiguous terms   */

    /* Allocate or expand storage to hold up to index n */
    if (n >= computed_len) {
        /* Grow capacity as needed */
        while (allocated <= n) {
            idx_t new_alloc = allocated == 0 ? 1 : allocated * 2;
            if (new_alloc <= n) new_alloc = n + 1;

            /* realloc returns void*, cast explicitly for C++ */
            seq_t *tmp_mem = (seq_t *)realloc(memo, new_alloc * sizeof(*tmp_mem));
            unsigned char *tmp_comp = (unsigned char *)realloc(computed,
                                    new_alloc * sizeof(*tmp_comp));
            if (!tmp_mem || !tmp_comp) abort();   /* allocation failed */

            /* Initialise the new part of the flag array */
            for (idx_t i = allocated; i < new_alloc; ++i) {
                tmp_comp[i] = 0;
            }

            memo = tmp_mem;
            computed = tmp_comp;
            allocated = new_alloc;
        }

        /* Compute all terms from the first uncomputed index up to n */
        for (idx_t i = computed_len; i <= n; ++i) {
            if (i == 0) {
                memo[0] = 1;                     /* a(0) = 1 */
            } else {
                seq_t sum = 0;
                idx_t maxk = (i - 1) / 5;
                for (idx_t k = 0; k <= maxk; ++k) {
                    sum += memo[k] * memo[i - 5 * k - 1];
                }
                memo[i] = -sum;                  /* recurrence */
            }
            computed[i] = 1;                    /* mark as computed */
        }
        computed_len = n + 1;
    }

    return memo[n];
}

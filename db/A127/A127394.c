/*
 * A127394  Number of irreducible representations of Sp(2n,R) with same
 *          infinitesimal character as the trivial representation.
 * https://oeis.org/A127394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=4, a(2)=18, a(n)=4*a(n-1)+2*(n-1)*a(n-2) for n>=3 */

#include <stdlib.h>

/* cache of already-computed terms */
static seq_t *a_cache   = NULL;   /* array indexed from 1 (a_cache[0] unused) */
static idx_t  cache_cap = 0;      /* allocated capacity (number of elements)   */
static idx_t  cache_last= 0;      /* highest index that has been computed       */

/* Computes the n-th term of OEIS sequence A127394 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Ensure enough capacity for indices 0..n */
    if ((idx_t)n + 1 > cache_cap) {
        idx_t  new_cap = cache_cap > 0 ? cache_cap * 2 : 4;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;       /* allocation failure */
        a_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i == 1) {
            a_cache[1] = (seq_t)4;
        } else if (i == 2) {
            a_cache[2] = (seq_t)18;
        } else {
            seq_t prev1 = a_cache[i - 1];
            seq_t prev2 = a_cache[i - 2];
            seq_t term  = 4 * prev1 + 2 * ((seq_t)(i - 1)) * prev2;
            a_cache[i] = term;
        }
    }
    cache_last = n;

    return a_cache[n];
}

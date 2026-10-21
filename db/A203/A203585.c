/*
 * A203585  Vandermonde sequence using x^2 + y^2 applied to (2,3,...,n+1).
 * https://oeis.org/A203585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{2 ≤ i < j ≤ n+1} (i^2 + j^2), with a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static cache of already computed terms (index 1..max_cached) */
static seq_t *cache = NULL;
static idx_t  max_cached = 0;

/* Compute the n-th term of OEIS sequence A203585 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1, return 0 */

    /* Allocate or grow cache to hold a(n) */
    if (cache == NULL || n > max_cached) {
        size_t newsize = (size_t)n + 1;          /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) abort();                      /* allocation failed */
        cache = tmp;

        /* Initialize the first term, if not yet done */
        if (max_cached == 0) {
            cache[1] = (seq_t)1;                /* a(1) = 1 */
            max_cached = 1;
        }

        /* Compute missing terms up to n */
        while (max_cached < n) {
            idx_t next = max_cached + 1;        /* new value of n to compute */
            seq_t prod = cache[max_cached];     /* a(n-1) */

            /* New element corresponding to j = next + 1 */
            seq_t new_elem = (seq_t)next + 1;
            seq_t j2 = new_elem * new_elem;    /* (j)^2 */

            /* Multiply by all new factors (i^2 + j^2) for i = 2..next */
            for (idx_t i = 2; i <= next; ++i) {
                seq_t ii = (seq_t)i * (seq_t)i;   /* i^2 */
                prod *= (ii + j2);
            }

            cache[next] = prod;         /* store a(next) */
            max_cached = next;
        }
    }

    return cache[n];
}

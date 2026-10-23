/*
 * A176645  a(n+1) = m + Sum_{j=0..n} (a(j)*a(n-j) + k) for n>=1, with a(0)=1,
 *          a(1)=4, k=1 and m=1.
 * https://oeis.org/A176645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A176645.
   Recurrence (with k = 1, m = 1):
        a(0) = 1
        a(1) = 4
        a(n) = (∑_{j=0}^{n-1} a(j) * a(n-1-j)) + n + 1   for n ≥ 2
   Indexing follows the OEIS offset (a(0) is the first term).
*/

#include <stdlib.h>

static seq_t *cache = NULL;      /* sequence values */
static idx_t  cache_len = 0;     /* allocated array length */
static idx_t  max_computed = 0;  /* highest index that has been computed */

/* Return a(n), computing any missing terms on demand.
   The function is reentrant with respect to different values of n. */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold a(n) */
    if (n >= cache_len) {
        idx_t new_len = cache_len ? cache_len * 2 : 4;
        while (new_len <= n) new_len *= 2;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }

    /* Initialise base values if they have not yet been set */
    if (max_computed == 0) {
        cache[0] = (seq_t)1;
        max_computed = 0;
    }
    if (n >= 1 && max_computed < 1) {
        cache[1] = (seq_t)4;
        max_computed = 1;
    }

    /* Compute all missing entries up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* The recurrence is valid only for i ≥ 2 */
        seq_t sum = 0;
        for (idx_t j = 0; j < i; ++j) {
            sum += cache[j] * cache[i - 1 - j];
        }
        sum += (seq_t)i + (seq_t)1;   /* add k*i + m (k = m = 1) */
        cache[i] = sum;
    }
    max_computed = n;

    return cache[n];
}

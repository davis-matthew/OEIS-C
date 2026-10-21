/*
 * A215450  a(0)=0, a(1)=1, a(n) = a(n-1) + (Sum_{i=0..n-1} a(i)) mod n.
 * https://oeis.org/A215450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0,
   a(1) = 1,
   a(n) = a(n-1) + (sum_{i=0}^{n-1} a(i)) mod n   for n >= 2
*/

#include <stdlib.h>

/* Computes A215450.  The result is cached so that out–of–order
   calls are handled efficiently.  */
seq_t a(idx_t n)
{
    /* Static state used for memoisation */
    static seq_t *cache   = NULL;  /* array of computed terms   */
    static idx_t  cap     = 0;     /* current allocation size   */
    static idx_t  len     = 0;     /* number of terms stored    */
    static seq_t  cum_sum = 0;     /* Σ_{i=0}^{len-1} a(i)      */

    /* Helper to grow the cache when needed */
    if (n >= cap) {
        size_t new_cap = cap ? cap * 2 : 4;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory: terminate */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the cache with the first two terms, if required */
    if (len == 0) {
        cache[0] = 0;
        cache[1] = 1;
        cum_sum  = 1;   /* 0 + 1 */
        len = 2;
    }

    /* Return the term if it is already known */
    if (n < len) return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        /* a(i) = a(i-1) + (sum_{j=0}^{i-1} a(j)) mod i  */
        seq_t prev   = cache[i - 1];
        seq_t mod_i  = cum_sum % (seq_t)i;
        seq_t val    = prev + mod_i;

        cache[i] = val;
        cum_sum += val;
    }
    len = n + 1;

    return cache[n];
}

/*
 * A028678  Pseudo Galois numbers for d=16; order of group AGL(n,2^4).
 * https://oeis.org/A028678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16^(n(n+1)/2) * Π_{j=1}^n (16^j - 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;             /* Cached values of a(n)             */
static idx_t   cache_size = 0;            /* Current size of a_cache (in seq_t)*/
static idx_t   last_n    = 0;             /* Highest n that has been computed  */
static seq_t   pow_q_n   = (seq_t)1;      /* q^last_n  (q = 16)               */
static seq_t   cur_powq  = (seq_t)1;      /* q^(last_n*(last_n+1)/2)          */
static seq_t   prod_b    = (seq_t)1;      /* Π_{j=1}^{last_n} (q^j - 1)       */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold a(n) */
    if (n >= cache_size) {
        /* Grow the cache */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – terminate with a diagnostic */
            abort();
        }
        a_cache = tmp;
        cache_size = new_size;

        /* If this is the first time we are initializing, set a(0) = 1 */
        if (new_size == 1) {
            a_cache[0] = (seq_t)1;
        }
    }

    /* If requested value already computed, return it */
    if (n <= last_n) {
        return a_cache[n];
    }

    /* Compute values for n = last_n+1 .. n */
    const seq_t q = (seq_t)16;
    for (idx_t k = last_n + 1; k <= n; ++k) {
        /* pow_q_n = q^k ;  cur_powq = q^(k(k+1)/2) */
        pow_q_n  *= q;      /* q^k */
        cur_powq *= pow_q_n;/* multiply by q^k */

        /* prod_b = Π_{j=1}^k (q^j - 1) */
        prod_b *= (pow_q_n - (seq_t)1);

        /* a(k) = cur_powq * prod_b */
        a_cache[k] = cur_powq * prod_b;
    }

    last_n = n;
    return a_cache[n];
}

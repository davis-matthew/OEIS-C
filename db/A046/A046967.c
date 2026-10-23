/*
 * A046967  a(1) = 2, a(n) = a(n-1)^prime(n) for n > 1.
 * https://oeis.org/A046967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 2; a(n) = a(n-1)^prime(n) for n > 1 */

/* Utility function declarations */
#include "../utils/big_pow.h"
#include "../utils/get_prime.h"

#include <stddef.h>
#include <stdlib.h>

/* Sequence accessor */
seq_t a(idx_t n)
{
    /* Cache for already computed terms.
     * cache[i] holds a(i+1); computed terms = 0..computed-1
     */
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* allocated capacity in terms   */
    static idx_t computed = 0;  /* number of terms already computed */

    if (n < 1)
        return (seq_t)0;        /* undefined for indices below 1 */

    /* Ensure sufficient storage */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)            /* allocation failed */
            exit(EXIT_FAILURE);
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)2;
        } else {
            seq_t base = cache[i - 2];                /* a(i-1)   */
            seq_t exp  = (seq_t)get_prime(i);         /* prime(i) */
            cache[i - 1] = big_pow(base, exp);        /* a(i)     */
        }
    }
    computed = n;
    return cache[n - 1];
}

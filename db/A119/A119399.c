/*
 * A119399  a(n) = Sum_{k=0..n} (n!/k!)^2*binomial(n-1,k-1).
 * https://oeis.org/A119399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} ((n!/k!)^2 * binomial(n-1,k-1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compiles under C11 and C++17; seq_t and idx_t are provided by the build.   */

#include <stdlib.h>

/* Static cache of computed values.  */
static seq_t *cache = NULL;      /* array of a(i) for 0 <= i < cache_size */
static idx_t   cache_size = 0;   /* number of valid entries in cache */

/* Ensure the cache contains values up to and including n.  */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)         /* already computed */
        return;

    /* Allocate or grow the cache.  */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* In a real program we would handle allocation failure.
         * For the purposes of the kata we simply abort. */
        abort();
    }

    /* Compute new entries.  */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i == 0) {
            new_cache[i] = (seq_t)1;
        } else if (i == 1) {
            new_cache[i] = (seq_t)1;
        } else if (i == 2) {
            new_cache[i] = (seq_t)5;
        } else {
            /* Recurrence:
             * a(n) = (3n²-5n+3) a(n-1)
             *      + (n-3)(n-1)²(n-2)³ a(n-3)
             *      - (n-1)²(3n-4)(n-2) a(n-2)              */
            seq_t coeff1 = (seq_t)3 * (seq_t)i * (seq_t)i
                           - (seq_t)5 * (seq_t)i
                           + (seq_t)3;

            seq_t coeff2 = (seq_t)(i-3)
                           * (seq_t)(i-1)
                           * (seq_t)(i-1)
                           * (seq_t)(i-2)
                           * (seq_t)(i-2)
                           * (seq_t)(i-2);

            seq_t coeff3 = (seq_t)(i-1)
                           * (seq_t)(i-1)
                           * ((seq_t)3 * (seq_t)i - (seq_t)4)
                           * (seq_t)(i-2);

            seq_t term1 = coeff1 * new_cache[i-1];
            seq_t term2 = coeff2 * new_cache[i-3];
            seq_t term3 = coeff3 * new_cache[i-2];

            new_cache[i] = term1 + term2 - term3;
        }
    }

    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

/*
 * A038559  a(n) = 2*A040027(n-1) + Bell(n), where Bell = A000110.
 * https://oeis.org/A038559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n-1,k-1)*a(n-k) a(0) = 1, a(1) = 3
 *            (OEIS A038559)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;           /* currently allocated length */

    size_t target = (size_t)n;              /* index to compute */

    /* Allocate or grow the cache so it can hold indices up to n. */
    if (target + 1 > cache_size) {
        size_t new_size = target + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();           /* allocation failure */
        cache = new_cache;

        /* Initialise the first two terms (safe to overwrite existing ones). */
        if (new_size > 0) cache[0] = (seq_t)1;   /* a(0) */
        if (new_size > 1) cache[1] = (seq_t)3;   /* a(1) */

        /* Compute new terms that are not yet present.
           We start at index 2 if it hasn't been computed yet. */
        size_t i_start = (cache_size < 2) ? 2 : cache_size;
        for (size_t i = i_start; i <= target; ++i) {
            seq_t sum = (seq_t)0;
            seq_t bin = (seq_t)1;          /* binomial coefficient C(i-1,0) */

            for (size_t j = 0; j < i; ++j) {
                sum += bin * cache[i - 1 - j];
                if (j + 1 == i) break;     /* last coefficient is 1 */
                bin = bin * (seq_t)(i - 1 - j) / (seq_t)(j + 1);
            }
            cache[i] = sum;
        }

        cache_size = new_size;
    }

    return cache[target];
}

/*
 * A038125  a(n) = Sum_{k=0..n} (k-n)^k.
 * https://oeis.org/A038125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} (k-n)^k
 * Range:     i8 n <= 10, i16 n <= 14, i32 n <= 22, i64 n <= 32, i128 n <= 51,
 *            big any n
 * Helpers:   seq_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_realloc.h"
#include "../utils/seq_pow.h"

/* Computes the OEIS sequence A038125 */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Extend cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        cache = seq_realloc(cache, cache_size, new_size);

        /* Compute sequence values that are not yet cached */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k <= i; ++k) {
                seq_t base = (seq_t)k - (seq_t)i;      /* (k - i) */
                seq_t term = seq_pow(base, k);          /* (k-i)^k  */
                sum = sum + term;
            }
            cache[i] = sum;
        }
        cache_size = new_size;
    }

    return cache[n];
}

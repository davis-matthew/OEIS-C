/*
 * A100722  Prime numbers whose binary representations are split into exactly
 *          five runs.
 * https://oeis.org/A100722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = nth prime whose binary representation splits into exactly
 *            five runs
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binary_runs, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binary_runs.h"
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;
    static uint64_t candidate = 2;

    if (n == 0)
        return 0; /* undefined for offset 1 */

    while (cache_size < n) {
        /* test current candidate */
        if (is_prime_u64(candidate) && binary_runs(candidate) == 5) {
            /* grow cache if needed */
            if (cache_size == cache_cap) {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort(); /* memory allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)candidate;
        }

        /* advance candidate to next odd number */
        candidate = (candidate == 2) ? 3 : candidate + 2;
    }

    return cache[n - 1];
}

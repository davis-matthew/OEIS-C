/*
 * A172112  Partial sums of A023200.
 * https://oeis.org/A172112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the first n primes p such that p and p+4 are prime
 *            (the smaller member of each cousin‑prime pair). The function
 *            uses incremental sieving and caches the partial sums so that
 *            repeated calls with any order of n are answered in O(n) time
 *            overall
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* behaviour below offset is irrelevant */

    /* static cache for partial sums */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* static state for the incremental search */
    static idx_t candidate = 3;      /* odd numbers only */
    static seq_t sum = 0;            /* running total */

    /* grow cache if needed */
    if (cache_len < (size_t)n) {
        if (cache_cap == 0) {
            cache_cap = 16;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* continue searching for cousin primes until cache has n terms */
        while (cache_len < (size_t)n) {
            /* Only odd candidates are possible primes > 2 */
            if (is_prime_small((seq_t)candidate) &&
                is_prime_small((seq_t)(candidate + 4))) {
                sum += (seq_t)candidate;
                cache[cache_len++] = sum;
            }
            candidate += 2;         /* next odd number */
        }
    }

    return cache[n - 1];
}

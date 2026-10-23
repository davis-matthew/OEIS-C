/*
 * A129401  a(n) is the result of replacing with its successor prime each
 *          prime in the factorization of the n-th composite number.
 * https://oeis.org/A129401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = result of replacing each prime factor of the n‑th
 *            composite number with its successor prime. This is A125901 in
 *            OEIS
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, is_prime, next_prime, seq_alloc_new_array,
 *            seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* compute a(n) for a given composite number m */
static seq_t compute(uint64_t m) {
    uint64_t primes[64], exp[64];
    size_t cnt = 0;
    factor_u64(m, primes, exp, &cnt);
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t sp = next_prime(p);
        for (uint64_t j = 0; j < exp[i]; ++j)
            prod *= (seq_t)sp;
    }
    return prod;
}

/* static cache data */
static seq_t *cache = NULL;
static size_t cache_sz = 0;
static uint64_t last_num = 3;   /* last composite number examined */
static idx_t last_count = 0;    /* number of composites cached */

/* public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* not used – offset starts at 1 */

    /* enlarge cache if necessary */
    if ((size_t)n > cache_sz) {
        size_t new_sz = (size_t)n;
        seq_t *new_cache = seq_alloc_new_array(new_sz);
        /* copy old values */
        for (size_t i = 0; i < cache_sz; ++i)
            new_cache[i] = cache[i];
        /* free old array */
        if (cache) seq_free_array(cache);
        cache = new_cache;
        cache_sz = new_sz;

        /* compute missing values */
        for (idx_t i = last_count + 1; i <= n; ++i) {
            /* find next composite number */
            do {
                ++last_num;
            } while (last_num < 4 || is_prime(last_num));
            cache[i - 1] = compute(last_num);
        }
        last_count = n;
    }

    return cache[n - 1];
}

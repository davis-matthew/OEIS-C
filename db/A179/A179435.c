/*
 * A179435  For positive n with prime decomposition n = Product_{j=1..m}
 *          (p_j^k_j) define a_n = Sum_{j=1..m} (p_j*k_j) and b_n =
 *          Sum_{j=1..m} (p_j^k_j). This sequence gives those n for which a_n
 *          and b_n are both prime and unequal.
 * https://oeis.org/A179435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer n (1‑based) such that, for its prime
 *            factorization n = ∏p_j^k_j, a_n = Σp_j*k_j and b_n = Σp_j^k_j
 *            are both prime and a_n ≠ b_n. The sequence is generated on
 *            demand and cached
 * Range:     u8 n <= 7, u16 n <= 434, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, is_prime_big, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    /* Last integer examined. */
    static uint64_t tested = 1;

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        tested++; /* candidate integer */
        uint64_t m = tested;

        /* Factor m into primes and exponents. */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t factor_cnt = 0;
        factor_u64(m, primes, exps, &factor_cnt);

        /* Compute a_sum and b_sum as seq_t. */
        seq_t a_sum = 0;
        seq_t b_sum = 0;
        for (size_t i = 0; i < factor_cnt; ++i) {
            seq_t p = (seq_t)primes[i];
            idx_t k = (idx_t)exps[i];
            a_sum += p * k;
            seq_t pk = seq_pow(p, k);
            b_sum += pk;
        }

        /* Check the defining condition. */
        if (a_sum != b_sum && is_prime_big(a_sum) && is_prime_big(b_sum)) {
            /* Expand the cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failure */
                    free(cache);
                    cache = NULL;
                    cache_cap = cache_len = 0;
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
    }
    return cache[n - 1];
}

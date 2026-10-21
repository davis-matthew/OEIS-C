/*
 * A095404  Numbers whose sum of digits is greater than the sum of the digits
 *          of all of its distinct prime factors.
 * https://oeis.org/A095404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer k whose sum of decimal digits
 *            exceeds * the sum of decimal digits of its distinct prime
 *            factors
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, factor_u64, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies: digit_sum, factor_u64, grow_seq */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/factor_u64.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    /* Last integer checked (next candidate to test) */
    static idx_t last_checked = 1;

    /* Ensure cache can hold at least n terms */
    if (n > cache_cap) {
        grow_seq(&cache, &cache_cap, n);
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < n) {
        /* Test current number last_checked */
        uint64_t k = (uint64_t)last_checked;

        /* Sum of decimal digits of k */
        seq_t ds = digit_sum((seq_t)k);

        /* Factor k into distinct prime factors */
        size_t factor_cnt = 0;
        uint64_t primes[64], exps[64];
        factor_u64(k, primes, exps, &factor_cnt);

        /* Sum of decimal digits of distinct prime factors */
        seq_t sum_pf_digits = (seq_t)0;
        for (size_t i = 0; i < factor_cnt; ++i) {
            sum_pf_digits += digit_sum((seq_t)primes[i]);
        }

        /* Check condition */
        if (ds > sum_pf_digits) {
            /* Store the term in the cache */
            if (cache_size >= cache_cap) {
                grow_seq(&cache, &cache_cap, cache_size + 1);
            }
            cache[cache_size++] = (seq_t)k;
        }

        ++last_checked;  /* move to next integer */
    }

    return cache[n - 1];
}

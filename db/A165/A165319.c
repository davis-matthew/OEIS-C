/*
 * A165319  Primes p where the number of divisors of p+1 is a power of 2.
 * https://oeis.org/A165319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime p such that the number of divisors of
 *            (p+1) is a power of 2
 * Range:     u8 n <= 24, u16 n <= 3271, u32 n <= 9306, u64 n <= 9306,
 *            u128 n <= 9306, big any n
 * Helpers:   divisor_count_u64, is_power_of_two, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t last_checked = 1;  // number up to which we have examined

    if ((size_t)n <= cache_len) {
        return cache[(size_t)n - 1];
    }

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < (size_t)n) {
        last_checked++;
        if (is_prime(last_checked)) {
            uint64_t p_plus1 = last_checked + 1;
            size_t div_cnt = divisor_count_u64(p_plus1);
            seq_t d_seq = (seq_t)div_cnt;
            if (is_power_of_two(d_seq)) {
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)last_checked;
            }
        }
    }
    return cache[(size_t)n - 1];
}

/*
 * A057258  Numbers n such that n | 11^n + 10^n + 9^n + 8^n + 7^n + 6^n.
 * https://oeis.org/A057258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer such that n | 11^n+10^n+9^n+8^n+7^n+6^n
 * Range:     u8 n <= 9, u16 n <= 39, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_len = 0;
    static idx_t next_to_test = 1;          /* next integer to examine */

    /* obtain enough terms */
    while (cache_len < (size_t)n) {
        /* enlarge cache if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
            if (new_cap < cache_len + 1) new_cap = cache_len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* test next integer */
        seq_t sum = 0;
        seq_t mod = (seq_t)next_to_test;

        for (int base = 6; base <= 11; ++base) {
            seq_t r = mod_pow((seq_t)base, (idx_t)next_to_test, mod);
            sum = (sum + r) % mod;
        }

        if (sum == 0)
            cache[cache_len++] = (seq_t)next_to_test;

        ++next_to_test;
    }

    return cache[n - 1];
}

/*
 * A224217  Numbers b such that b^2 + 4*b + 9 is prime.
 * https://oeis.org/A224217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 14, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A224217: a(n) is the nth integer b for which
   b^2 + 4*b + 9 is prime. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* offset 1 – safety fallback */
        return 0;
    }

#ifdef __cplusplus
    static std::vector<seq_t> cache_vec;
    static idx_t next_b = 1;

    while (cache_vec.size() < (size_t)n) {
        seq_t b = (seq_t)next_b;
        seq_t f = b * b + (seq_t)4 * b + (seq_t)9;
        if (is_prime_big(f)) {
            cache_vec.push_back(b);
        }
        ++next_b;
    }

    return cache_vec[n - 1];
#else
    static seq_t *cache_ptr = NULL;
    static size_t sz = 0;
    static size_t cap = 0;
    static idx_t next_b = 1;

    while (sz < (size_t)n) {
        seq_t b = (seq_t)next_b;
        seq_t f = b * b + (seq_t)4 * b + (seq_t)9;
        if (is_prime_big(f)) {
            if (cap == sz) {
                size_t new_cap = cap ? cap << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(cache_ptr, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache_ptr = tmp;
                cap = new_cap;
            }
            cache_ptr[sz++] = b;
        }
        ++next_b;
    }

    return cache_ptr[n - 1];
#endif
}

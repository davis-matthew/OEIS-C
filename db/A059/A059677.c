/*
 * A059677  Numbers n such that 1n1, 3n3, 7n7 and 9n9 are all primes.
 * https://oeis.org/A059677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   numbers n such that 1n1, 3n3, 7n7 and 9n9 are all primes
 * Range:     u8 none, u16 n <= 2, u32 n <= 2, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 1;

    while (cache_len < n) {
        seq_t cand = next_candidate++;
        int d = digit_len(cand);
        seq_t pow10_val = pow10((idx_t)(d + 1));
        bool ok = true;
        const seq_t prefixes[4] = {1, 3, 7, 9};

        for (int i = 0; ok && i < 4; ++i) {
            seq_t p = prefixes[i];
            seq_t part = cand * ((seq_t)10) + p;
            seq_t num = p * pow10_val + part;
            if (!is_prime_big(num)) {
                ok = false;
            }
        }

        if (ok) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}

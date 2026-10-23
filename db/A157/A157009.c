/*
 * A157009  Primes p such that 3 = abs(largest digit of p - sum of all the
 *          other digits of p).
 * https://oeis.org/A157009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime p such that 3 = abs(max digit of p - sum of
 *            all the other digits of p)
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   abs_int, digit_counts, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/digit_counts.h"
#include "../utils/abs_int.h"

seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_len = 0;
    if (n == 0) return 0;  /* undefined, but avoid UB */

    while (cache_len < n) {
        seq_t candidate = (cache_len == 0) ? 2 : cache[cache_len - 1] + 1;
        while (1) {
            bool prime;
            if (sizeof(seq_t) <= 8) {
                prime = is_prime_small((uint64_t)candidate);
            } else {
                prime = is_prime_big(candidate);
            }
            if (!prime) {
                candidate++;
                continue;
            }

            unsigned char cnt[10] = {0};
            digit_counts(candidate, cnt);

            int sum = 0, maxd = 0;
            for (int d = 0; d <= 9; d++) {
                if (cnt[d]) {
                    sum += d * cnt[d];
                    if (d > maxd) maxd = d;
                }
            }
            int diff = abs_int(2 * maxd - sum);
            if (diff == 3) {
                /* append to cache */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;  /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = candidate;
                break;
            }
            candidate++;
        }
    }
    return cache[n - 1];
}

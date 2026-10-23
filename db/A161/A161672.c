/*
 * A161672  Half the product of all numbers from A141468(n) up to prime(n).
 * https://oeis.org/A161672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)! / (2 * (A141468(n)-1)!) where A141468(n) is the
 *            n-th non‑prime integer (including 0 and 1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/get_prime.h"

static idx_t get_nonprime(idx_t k)
{
    static idx_t *nonprimes = NULL;
    static idx_t non_cap = 0, non_cnt = 0, last_checked = 0;

    if (k == 0)
        return 0;

    if (k > non_cnt) {
        if (non_cap == 0) {
            non_cap = 64;
            nonprimes = (idx_t*)malloc(non_cap * sizeof(idx_t));
        }
        while (non_cnt < k) {
            if (last_checked <= 1) {
                /* Handle 0 and 1 as non‑prime */
                nonprimes[non_cnt] = last_checked;
                ++non_cnt;
                ++last_checked;
                continue;
            }
            ++last_checked;
            if (!is_prime(last_checked)) {
                if (non_cnt >= non_cap) {
                    non_cap *= 2;
                    nonprimes = (idx_t*)realloc(nonprimes, non_cap * sizeof(idx_t));
                }
                nonprimes[non_cnt] = last_checked;
                ++non_cnt;
            }
        }
    }
    return nonprimes[k - 1];
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0, cache_n = 0;

    if (n == 0) {
        /* Undefined for offset 0; return 0 or handle as needed */
        return (seq_t)0;
    }

    if (n > cache_n) {
        if (cache == NULL) {
            cache_cap = 64;
            cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        }
        for (idx_t k = cache_n + 1; k <= n; ++k) {
            idx_t low  = get_nonprime(k);
            idx_t high = get_prime(k);

            seq_t prod = (seq_t)1;
            for (idx_t i = low; i <= high; ++i) {
                prod *= (seq_t)i;
            }
            prod /= (seq_t)2;

            if (cache_n + 1 >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_n + 1] = prod;
            ++cache_n;
        }
    }
    return cache[n];
}

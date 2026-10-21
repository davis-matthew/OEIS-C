/*
 * A220746  Numbers n such that n and n + 10 are prime and there is a power of
 *          two in the interval (n, n+10).
 * https://oeis.org/A220746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer n such that n and n+10 are prime and
 *            there exists a power of two 2^k with n < 2^k < n+10
 * Range:     u8 none, u16 n <= 7, u32 n <= 7, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache for generated terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* state for generating new terms */
    static idx_t cur_k = 1;          /* current exponent of two   */
    static seq_t cur_power;          /* 2^cur_k */
    static seq_t last_n = 0;         /* last inserted term */

    /* initialise static data on first call */
    if (cache == NULL) {
        cache_cap   = 128;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len   = 0;
        cur_k       = 1;
        cur_power   = pow2(cur_k);   /* 2^1 = 2 */
        last_n      = 0;
    }

    /* generate terms until we have at least n entries */
    while (cache_len < n) {
        /* For the current power of two, test all possible offsets d
           such that the power lies between (2^k - d, 2^k + (10-d)). */
        for (int d = 9; d >= 1; --d) {
            seq_t cand = cur_power - d;
            if (cand < (seq_t)2)            /* candidate must be >= 2 */
                continue;
            seq_t cand10 = cur_power + ((seq_t)10 - (seq_t)d);

            if (is_prime_big(cand) && is_prime_big(cand10)) {
                if (cand > last_n) {       /* keep ascending order */
                    /* grow cache if necessary */
                    if (cache_len == cache_cap) {
                        cache_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
                        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_len++] = cand;
                    last_n = cand;
                    if (cache_len >= n)
                        break;
                }
            }
        }
        if (cache_len >= n)
            break;

        /* move to the next power of two */
        ++cur_k;
        cur_power = pow2(cur_k);
    }

    return cache[n - 1];
}

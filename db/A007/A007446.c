/*
 * A007446  Exponentiation of e.g.f. for primes.
 * https://oeis.org/A007446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n-1,k-1)*prime(k)*a(n-k), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 35,
 *            big any n
 * Helpers:   grow_zero, zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/zalloc_arr.h"
#include "../utils/grow_zero.h"

static uint64_t *pr_list = NULL;
static idx_t pr_count = 0;
static idx_t pr_cap = 0;

static uint64_t nth_prime(idx_t k) {
    while (pr_count < k) {
        if (pr_count >= pr_cap) {
            idx_t nc = pr_cap ? pr_cap * 2 : 1024;
            pr_list = (uint64_t *)realloc(pr_list, sizeof(uint64_t) * nc);
            pr_cap = nc;
        }
        if (pr_count == 0) {
            pr_list[pr_count++] = 2;
            continue;
        }
        uint64_t c = (pr_count == 1) ? 3 : pr_list[pr_count - 1] + 2;
        for (;;) {
            bool ok = true;
            for (idx_t j = 1; j < pr_count; j++) {
                uint64_t q = pr_list[j];
                if (q * q > c) break;
                if (c % q == 0) { ok = false; break; }
            }
            if (ok) break;
            c += 2;
        }
        pr_list[pr_count++] = c;
    }
    return pr_list[k - 1];
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static seq_t *row = NULL;   /* row[j] = binomial(cache_len-1, j) */
    static idx_t cap = 0;
    static idx_t cache_len = 0;

    if (cache == NULL) {
        cap = 1024;
        cache = zalloc_arr(cap);
        row = zalloc_arr(cap);
        cache[0] = 1;
        row[0] = 1;
        cache_len = 1;
    }

    while (cache_len <= n) {
        if (cache_len + 1 >= cap) {
            idx_t nc = cap * 2;
            cache = grow_zero(cache, cap, nc);
            row = grow_zero(row, cap, nc);
            cap = nc;
        }
        idx_t i = cache_len;
        seq_t result = 0;
        for (idx_t k = 1; k <= i; k++) {
            seq_t p = (seq_t)nth_prime(k);
            result += row[k - 1] * p * cache[i - k];
        }
        cache[i] = result;
        cache_len++;
        /* advance row from binomial(i-1, .) to binomial(i, .) */
        for (idx_t j = i; j >= 1; j--) {
            row[j] += row[j - 1];
        }
    }

    return cache[n];
}

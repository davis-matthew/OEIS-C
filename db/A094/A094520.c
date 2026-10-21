/*
 * A094520  Numbers such that all sums of two distinct divisors are not
 *          divisors.
 * https://oeis.org/A094520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sequence of integers m such that for all distinct divisors d1,d2
 *            of m, the sum d1+d2 does not divide m. a(1)=1, a(2)=2,
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool is_good(uint64_t m)
{
    /* collect all divisors of m in array d */
    uint64_t d[128];
    int dcnt = 0;
    for (uint64_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            d[dcnt++] = i;
            uint64_t j = m / i;
            if (j != i) {
                if (dcnt == 128) return false; /* implicit failure */
                d[dcnt++] = j;
            }
        }
    }
    /* check all unordered distinct pairs */
    for (int i = 0; i < dcnt; ++i) {
        for (int j = i + 1; j < dcnt; ++j) {
            uint64_t sum = d[i] + d[j];
            if (m % sum == 0) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 1;

    /* enlarge cache if needed */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* out of memory */
    }

    /* produce terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* find next good integer */
        while (!is_good(next_candidate))
            ++next_candidate;

        /* store it */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    /* return the nth term (a(1) corresponds to index 0) */
    return cache[n - 1];
}

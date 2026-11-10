/*
 * A099822  Bisection of A005117.
 * https://oeis.org/A099822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A005117(2*n-1), where A005117 is the squarefree numbers;
 *            computed by a search with a cached list
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool A005117_is_squarefree_u64(uint64_t k) {
    if (k % 4 == 0)
        return false;
    for (uint64_t i = 3; i * i <= k; i += 2) {
        if (k % (i * i) == 0)
            return false;
    }
    return true;
}

static uint64_t A005117_a(uint64_t n) {
    static uint64_t *cache = 0;
    static uint64_t cache_size = 0;
    static uint64_t cache_cap = 0;

    if (n > cache_size) {
        if (n > cache_cap) {
            uint64_t newcap = cache_cap ? cache_cap : 64;
            while (newcap < n)
                newcap *= 2;
            cache = (uint64_t *)realloc(cache, (size_t)newcap * sizeof(uint64_t));
            cache_cap = newcap;
        }
        uint64_t k = cache_size ? cache[cache_size - 1] : 0;
        for (uint64_t i = cache_size; i < n; i++) {
            do {
                k++;
            } while (!A005117_is_squarefree_u64(k));
            cache[i] = k;
        }
        cache_size = n;
    }

    return cache[n - 1];
}

seq_t a(idx_t n) {
    return (seq_t)A005117_a((uint64_t)(2 * n - 1));
}

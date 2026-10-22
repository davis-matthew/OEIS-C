/*
 * A005117  Squarefree numbers: numbers that are not divisible by a square
 *          greater than 1.
 * https://oeis.org/A005117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   search — a(n) is the n-th k >= 1 such that no p^2 (p > 1)
 *            divides k
 * Range:     u8 n <= 157, u16 n <= 9631, u32 n <= 9631, u64 n <= 9631,
 *            u128 n <= 9631, big any n
 * Helpers:   is_squarefree_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_squarefree_u64.h"

seq_t a(idx_t n) {
    static uint64_t *cache = 0;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;

    if (n > cache_size) {
        if (n > cache_cap) {
            idx_t newcap = cache_cap ? cache_cap : 64;
            while (newcap < n)
                newcap *= 2;
            cache = (uint64_t *)realloc(cache, (size_t)newcap * sizeof(uint64_t));
            cache_cap = newcap;
        }
        uint64_t k = cache_size ? cache[cache_size - 1] : 0;
        for (idx_t i = cache_size; i < n; i++) {
            do {
                k++;
            } while (!is_squarefree_u64(k));
            cache[i] = k;
        }
        cache_size = n;
    }

    return (seq_t)cache[n - 1];
}

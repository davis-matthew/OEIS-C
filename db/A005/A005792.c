/*
 * A005792  Positive numbers that are the sum of 2 squares or 3 times a
 *          square.
 * https://oeis.org/A005792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9549 (2026-11-01)
 * Helpers:   is_square_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = n-th positive x that is a sum of 2 squares (every prime p == 3 mod 4 divides x to an even power) or 3 times a square */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_square_u64.h"

static bool sum2sq_u64(uint64_t x) {
    uint64_t p;
    while (x % 2 == 0) x /= 2;
    for (p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) { x /= p; e++; }
            if ((p % 4 == 3) && (e & 1)) return false;
        }
    }
    if (x > 1 && x % 4 == 3) return false;
    return true;
}

static bool in_seq(uint64_t x) {
    if (x == 0) return false;
    if (x % 3 == 0 && is_square_u64(x / 3)) return true;
    return sum2sq_u64(x);
}

seq_t a(idx_t n) {
    static uint64_t *cache = NULL;
    static idx_t len = 0, cap = 0;
    static uint64_t last = 0;

    while (len < n) {
        uint64_t x = last + 1;
        last = x;
        if (in_seq(x)) {
            if (len >= cap) {
                idx_t nc = cap ? cap * 2 : 1024;
                while (nc < n) nc *= 2;
                cache = (uint64_t *)realloc(cache, (size_t)nc * sizeof(uint64_t));
                cap = nc;
            }
            cache[len++] = x;
        }
    }
    return (seq_t)cache[n - 1];
}

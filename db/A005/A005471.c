/*
 * A005471  Primes of the form m^2 + 3m + 9, where m can be positive or
 *          negative.
 * https://oeis.org/A005471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 67, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = n-th prime of the form m^2 + m + 7 with m >= 0 (equivalently m^2 + 3m + 9, m in Z); m^2+m+7 is increasing in m, so terms appear in order */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static uint64_t *vals = NULL;
static idx_t cnt = 0;
static idx_t cap = 0;
static uint64_t next_m = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while (cnt < n) {
        uint64_t m = next_m++;
        uint64_t v = m * m + m + 7;
        if (is_prime(v)) {
            if (cnt == cap) {
                idx_t ncap = cap ? cap * 2 : 1024;
                uint64_t *p = (uint64_t *)realloc(vals, (size_t)ncap * sizeof(uint64_t));
                if (!p) return 0;
                vals = p;
                cap = ncap;
            }
            vals[cnt++] = v;
        }
    }
    return (seq_t)vals[n - 1];
}

/*
 * A007488  Primes whose reversal is a square.
 * https://oeis.org/A007488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 2822, u64 n <= 9492,
 *            u128 n <= 9492, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: for each digit length d, reverse every d-digit square s that does not end in 0, keep the reversals that are prime, sort them; a(n) is the n-th prime found */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t *list = NULL;
static size_t cnt = 0, cap = 0;
static uint64_t root = 1;
static uint64_t lim = 1;

static uint64_t reverse_digits(uint64_t x) {
    uint64_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void push(uint64_t v) {
    if (cnt == cap) {
        cap = cap ? cap * 2 : 1024;
        list = (uint64_t *)realloc(list, cap * sizeof(uint64_t));
    }
    list[cnt++] = v;
}

seq_t a(idx_t n) {
    while ((idx_t)cnt < n) {
        size_t start = cnt;
        lim *= 10;
        while (root * root < lim) {
            uint64_t s = root * root;
            root++;
            if (s % 10 == 0) continue;
            uint64_t p = reverse_digits(s);
            if (p > 1 && is_prime(p)) push(p);
        }
        qsort(list + start, cnt - start, sizeof(uint64_t), cmp_u64);
    }
    return (seq_t)list[n - 1];
}

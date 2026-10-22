/*
 * A001749  Primes multiplied by 4.
 * https://oeis.org/A001749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*prime(n)
 * Range:     u8 n <= 18, u16 n <= 1900, u32 n <= 9813, u64 n <= 9813,
 *            u128 n <= 9813, big any n
 * Helpers:   a001749_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/a001749_isprime.h"

static uint64_t *a001749_primes = NULL;
static idx_t a001749_count = 0;
static idx_t a001749_cap = 0;

static uint64_t a001749_nth_prime(idx_t n) {
    if (a001749_count >= n) return a001749_primes[n - 1];
    if (a001749_cap < n) {
        idx_t newcap = a001749_cap ? a001749_cap : 64;
        while (newcap < n) newcap *= 2;
        uint64_t *p = (uint64_t *)realloc(a001749_primes, (size_t)newcap * sizeof(uint64_t));
        if (!p) return 0;
        a001749_primes = p;
        a001749_cap = newcap;
    }
    uint64_t candidate;
    if (a001749_count == 0) {
        a001749_primes[0] = 2;
        a001749_count = 1;
    }
    candidate = a001749_primes[a001749_count - 1] + 1;
    while (a001749_count < n) {
        if (a001749_isprime(candidate)) {
            a001749_primes[a001749_count++] = candidate;
        }
        candidate++;
    }
    return a001749_primes[n - 1];
}

seq_t a(idx_t n) {
    uint64_t p = a001749_nth_prime(n);
    return (seq_t)p * 4;
}

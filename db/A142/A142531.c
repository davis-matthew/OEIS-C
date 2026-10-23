/*
 * A142531  Primes congruent to 51 mod 52.
 * https://oeis.org/A142531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 268, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A142531: primes congruent to 51 mod 52 (checked by primality test) */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static seq_t *specialPrimes = NULL;
static size_t specialCap = 0;
static size_t specialLen = 0;
static idx_t last_k = 0;

static void add_prime(seq_t value) {
    if (specialLen == specialCap) {
        size_t newCap = (specialCap == 0) ? 8 : specialCap * 2;
        seq_t *newArr = (seq_t *)realloc(specialPrimes, newCap * sizeof(seq_t));
        if (!newArr) abort();
        specialPrimes = newArr;
        specialCap = newCap;
    }
    specialPrimes[specialLen++] = value;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while (specialLen < (size_t)n) {
        ++last_k;
        /* compute candidate 52*k + 51 using 64‑bit arithmetic to avoid overflow */
        uint64_t temp = 52ull * (uint64_t)last_k + 51ull;
        seq_t cand = (seq_t)temp;
        if (is_prime((idx_t)temp)) {
            add_prime(cand);
        }
    }
    return specialPrimes[n - 1];
}

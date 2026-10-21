/*
 * A087340  Primes p such that the sum of the digits of p as well as 1 plus
 *          the product of its digits are also primes.
 * https://oeis.org/A087340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n‑th prime p such that digit_sum(p) and 1 +
 *            digit_product(p) are both prime numbers. The implementation
 *            searches primes sequentially, testing the above conditions until
 *            the n‑th term is found
 * Range:     u8 n <= 13, u16 n <= 794, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digitProduct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digitProduct.h"

/*-------------------- helper functions --------------------*/

static bool isPrime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    if (n % 5 == 0) return n == 5;
    if (n % 7 == 0) return n == 7;
    if (n % 11 == 0) return n == 11;
    if (n % 13 == 0) return n == 13;
    if (n % 17 == 0) return n == 17;
    if (n % 19 == 0) return n == 19;
    if (n % 23 == 0) return n == 23;
    if (n % 29 == 0) return n == 29;
    if (n % 31 == 0) return n == 31;
    if (n % 37 == 0) return n == 37;
    if (n % 41 == 0) return n == 41;
    if (n % 43 == 0) return n == 43;
    if (n % 47 == 0) return n == 47;
    if (n % 53 == 0) return n == 53;
    /* deterministic Miller–Rabin for 64‑bit integers */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        __uint128_t x = 1, p = d, base = a % n;
        while (p) {
            if (p & 1) x = (x * base) % n;
            base = (base * base) % n;
            p >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static uint64_t digitSum(uint64_t x)
{
    uint64_t s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/*-------------------- sequence implementation --------------------*/

static seq_t *cache = NULL;
static size_t cached = 0, cap = 0;
static uint64_t last_checked = 1;

seq_t a(idx_t n)
{
    if (cache == NULL) {
        cap = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();
    }
    while (cached < (size_t)n) {
        last_checked++;
        if (!isPrime(last_checked)) continue;
        uint64_t s = digitSum(last_checked);
        if (!isPrime(s)) continue;
        uint64_t p = digitProduct(last_checked);
        if (!isPrime(p + 1)) continue;
        if (cached == cap) {
            cap *= 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (cache == NULL) abort();
        }
        cache[cached++] = (seq_t)last_checked;
    }
    return cache[(size_t)n - 1];
}

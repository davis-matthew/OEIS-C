/*
 * A006883  Long period primes: the decimal expansion of 1/p has period p-1.
 * https://oeis.org/A006883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: the n-th prime p with ord_p(10) = p-1 (p = 2 included), tested by checking 10^((p-1)/q) != 1 mod p for each prime q | p-1 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t powmod_u64(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = (uint64_t)(((unsigned __int128)r * b) % m);
        b = (uint64_t)(((unsigned __int128)b * b) % m);
        e >>= 1;
    }
    return r;
}

static bool is_prime_u64(uint64_t m) {
    uint64_t d;
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (d = 3; d * d <= m; d += 2)
        if (m % d == 0) return false;
    return true;
}

static bool is_long_period(uint64_t p) {
    uint64_t m, q;
    if (p == 2) return true;
    if (p == 5) return false;
    m = p - 1;
    for (q = 2; q * q <= m; q++) {
        if (m % q == 0) {
            if (powmod_u64(10, (p - 1) / q, p) == 1) return false;
            while (m % q == 0) m /= q;
        }
    }
    if (m > 1 && powmod_u64(10, (p - 1) / m, p) == 1) return false;
    return true;
}

static uint64_t *terms = NULL;
static idx_t nterms = 0, capterms = 0;
static uint64_t last_cand = 1;

seq_t a(idx_t n) {
    while (nterms < n) {
        uint64_t c = ++last_cand;
        if (is_prime_u64(c) && is_long_period(c)) {
            if (nterms == capterms) {
                idx_t nc = capterms ? capterms * 2 : 1024;
                terms = (uint64_t *)realloc(terms, nc * sizeof(uint64_t));
                capterms = nc;
            }
            terms[nterms++] = c;
        }
    }
    return (seq_t)terms[n - 1];
}

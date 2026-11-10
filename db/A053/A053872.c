/*
 * A053872  Primes arising in A053782.
 * https://oeis.org/A053872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime of the form s(k) = sum of the first k
 *            composite numbers (primes arising in A053782); computed by a
 *            running search with Miller-Rabin primality
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 4303, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 54 terms through n = 9881 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static inline uint64_t A053872_mulmod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((unsigned __int128)a * b % mod);
}

static uint64_t A053872_powmod(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t r = 1;
    a %= mod;
    while (d) {
        if (d & 1) r = A053872_mulmod(r, a, mod);
        a = A053872_mulmod(a, a, mod);
        d >>= 1;
    }
    return r;
}

static int A053872_isprime(uint64_t n) {
    static const uint64_t sp[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    if (n < 2) return 0;
    for (size_t i = 0; i < sizeof(sp)/sizeof(sp[0]); ++i) {
        if (n == sp[i]) return 1;
        if (n % sp[i] == 0) return 0;
    }
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (size_t i = 0; i < sizeof(sp)/sizeof(sp[0]); ++i) {
        uint64_t x = A053872_powmod(sp[i], d, n);
        if (x == 1 || x == n - 1) continue;
        int ok = 0;
        for (unsigned r = 1; r < s; ++r) {
            x = A053872_mulmod(x, x, n);
            if (x == n - 1) { ok = 1; break; }
        }
        if (!ok) return 0;
    }
    return 1;
}

static uint64_t A053872_next = 4;
static uint64_t A053872_sum = 0;
static seq_t *A053872_cache = NULL;
static size_t A053872_len = 0;
static size_t A053872_cap = 0;

seq_t a(idx_t n)
{
    while (A053872_len < (size_t)n) {
        while (A053872_isprime(A053872_next)) ++A053872_next;
        A053872_sum += A053872_next;
        ++A053872_next;
        if (A053872_isprime(A053872_sum)) {
            if (A053872_len == A053872_cap) {
                size_t nc = A053872_cap ? A053872_cap * 2 : 16;
                A053872_cache = mem_resize(seq_t, A053872_cache, nc);
                A053872_cap = nc;
            }
            A053872_cache[A053872_len++] = (seq_t)A053872_sum;
        }
    }
    return A053872_cache[n - 1];
}

/*
 * A001134  Primes p such that the multiplicative order of 2 modulo p is
 *          (p-1)/4.
 * https://oeis.org/A001134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search - primes p == 1 (mod 8) (via a smallest-prime-factor
 *            sieve) with 2^((p-1)/4) == 1 (mod p) and 2^((p-1)/(4q)) != 1
 *            (mod p) for every prime q | (p-1)/4
 * Range:     u8 n <= 1, u16 n <= 317, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   s_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/s_powmod.h"

static uint64_t *s_terms = 0;
static size_t s_count = 0;
static uint64_t s_limit = 0;

static void s_build(uint64_t L) {
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint64_t i, j, p;
    size_t cap = 1024, cnt = 0;
    uint64_t *t = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= L)
                for (j = i * i; j <= L; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (p = 9; p <= L; p += 8) {
        uint64_t k, r;
        bool ok;
        if (spf[p] != p) continue;
        k = (p - 1) / 4;
        if (s_powmod(2, k, p) != 1) continue;
        ok = true;
        r = k;
        while (r > 1) {
            uint64_t q = spf[r];
            if (s_powmod(2, k / q, p) == 1) { ok = false; break; }
            while (r % q == 0) r /= q;
        }
        if (!ok) continue;
        if (cnt == cap) {
            cap *= 2;
            t = (uint64_t *)realloc(t, cap * sizeof(uint64_t));
        }
        t[cnt++] = p;
    }
    free(spf);
    free(s_terms);
    s_terms = t;
    s_count = cnt;
    s_limit = L;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    while ((uint64_t)s_count < n) {
        s_build(s_limit ? s_limit * 2 : (uint64_t)1 << 16);
    }
    return (seq_t)s_terms[n - 1];
}

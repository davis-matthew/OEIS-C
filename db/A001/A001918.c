/*
 * A001918  Least positive primitive root of n-th prime.
 * https://oeis.org/A001918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = least g >= 1 such that g^((p-1)/q) != 1 (mod p) for every
 *            prime q | p-1, where p = prime(n); found by a search over g
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a1918_factor, a1918_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a1918_factor.h"
#include "../utils/a1918_powmod.h"

static uint64_t *a1918_primes = NULL;
static idx_t a1918_count = 0;
static idx_t a1918_cap = 0;

static void a1918_extend(idx_t n) {
    uint64_t c;
    if (a1918_count >= n) return;
    if (a1918_cap < n) {
        idx_t nc = a1918_cap ? a1918_cap : 1024;
        while (nc < n) nc *= 2;
        a1918_primes = (uint64_t *)realloc(a1918_primes, (size_t)nc * sizeof(uint64_t));
        a1918_cap = nc;
    }
    if (a1918_count == 0) {
        a1918_primes[a1918_count++] = 2;
    }
    c = a1918_primes[a1918_count - 1] + 1;
    while (a1918_count < n) {
        int ok = 1;
        idx_t i;
        for (i = 0; i < a1918_count; i++) {
            uint64_t q = a1918_primes[i];
            if (q * q > c) break;
            if (c % q == 0) { ok = 0; break; }
        }
        if (ok) a1918_primes[a1918_count++] = c;
        c++;
    }
}

seq_t a(idx_t n) {
    uint64_t p, g, f[64];
    int nf, i;
    if (n < 1) return (seq_t)0;
    a1918_extend(n);
    p = a1918_primes[n - 1];
    nf = a1918_factor(p - 1, f);
    for (g = 1; g < p; g++) {
        int ok = 1;
        for (i = 0; i < nf; i++) {
            if (a1918_powmod(g, (p - 1) / f[i], p) == 1) { ok = 0; break; }
        }
        if (ok) return (seq_t)g;
    }
    return (seq_t)0;
}

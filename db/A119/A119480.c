/*
 * A119480  Numbers k such that the Bernoulli number B_{4k} has denominator
 *          30.
 * https://oeis.org/A119480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that for every prime p ≥ 7, * (p
 *            − 1) does NOT divide 4k. Equivalently, k is not a multiple of *
 *            (p − 1)/gcd(p − 1, 4) for any such prime
 * Range:     u8 n <= 32, u16 n <= 8841, u32 n <= 50000, u64 n <= 50000,
 *            u128 n <= 50000, big any n
 * Validated: standard, pass, 69 terms through n = 7598 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* By von Staudt-Clausen the denominator of B_{4k} is the product of the
   primes p with (p - 1) | 4k; 2, 3 and 5 always qualify, so it is 30 iff no
   prime p >= 7 does, i.e. k is not a multiple of (p - 1)/gcd(p - 1, 4) for
   any prime 7 <= p <= 4k + 1. The k up to a bound K are sieved that way and
   K doubles until there are n terms. (The original kept a fixed table of
   10000 terms and past it fell back to trial division of every p for every
   k, which timed out.) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *T = NULL;
    static size_t cnt = 0;
    static uint64_t K = 0;
    if (n == 0) return (seq_t)0;
    while (cnt < (size_t)n) {
        K = K ? 2 * K : 1 << 16;
        uint64_t lim = 4 * K + 1;
        unsigned char *comp = (unsigned char *)calloc(lim + 1, 1);
        unsigned char *bad = (unsigned char *)calloc(K + 1, 1);
        if (!comp || !bad) abort();
        for (uint64_t i = 2; i * i <= lim; ++i)
            if (!comp[i])
                for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
        for (uint64_t p = 7; p <= lim; ++p) {
            if (comp[p]) continue;
            uint64_t t = (p - 1) / ((p - 1) % 4 == 0 ? 4 : 2);
            for (uint64_t m = t; m <= K; m += t) bad[m] = 1;
        }
        size_t k = 0;
        for (uint64_t i = 1; i <= K; ++i) k += !bad[i];
        mem_free(T);
        T = mem_new(seq_t, k ? k : 1);
        if (!T) abort();
        cnt = 0;
        for (uint64_t i = 1; i <= K; ++i)
            if (!bad[i]) T[cnt++] = (seq_t)i;
        free(comp);
        free(bad);
    }
    return T[n - 1];
}

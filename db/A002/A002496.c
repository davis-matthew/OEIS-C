/*
 * A002496  Primes of the form k^2 + 1.
 * https://oeis.org/A002496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 + A005574(n)^2, i.e. the n-th prime of the form k^2+1.
 *            Found by a sieve over k: for each prime p <= K with p = 2 or p
 *            == 1 (mod 4), mark the k with k^2+1 == 0 (mod p) and k^2+1 != p;
 *            the unmarked k >= 1 give primes k^2+1. K is doubled until n
 *            terms are found
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 4615, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a002496_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a002496_powmod.h"

static uint64_t *a002496_ks = NULL;
static size_t a002496_cnt = 0;
static uint64_t a002496_K = 0;

static void a002496_build(uint64_t K) {
    unsigned char *isc = (unsigned char *)calloc((size_t)K + 1, 1);
    unsigned char *mark = (unsigned char *)calloc((size_t)K + 1, 1);
    uint64_t p, k;
    size_t c = 0;
    for (p = 2; p * p <= K; p++)
        if (!isc[p])
            for (k = p * p; k <= K; k += p) isc[k] = 1;
    /* p = 2: odd k > 1 give even k^2+1 > 2 */
    for (k = 3; k <= K; k += 2) mark[k] = 1;
    for (p = 5; p <= K; p += 4) {
        uint64_t r = 0, g, s;
        if (isc[p]) continue;
        for (g = 2; g < p; g++) {
            r = a002496_powmod(g, (p - 1) / 4, p);
            if (r * r % p == p - 1) break;
        }
        for (s = 0; s < 2; s++) {
            uint64_t start = s ? p - r : r;
            for (k = start; k <= K; k += p)
                if (k * k + 1 != p) mark[k] = 1;
        }
    }
    free(a002496_ks);
    a002496_ks = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)K + 1));
    for (k = 1; k <= K; k++)
        if (!mark[k]) a002496_ks[c++] = k;
    a002496_cnt = c;
    a002496_K = K;
    free(isc);
    free(mark);
}

seq_t a(idx_t n) {
    uint64_t K = a002496_K ? a002496_K : 512;
    uint64_t k;
    while (a002496_cnt < n) {
        K *= 2;
        a002496_build(K);
    }
    k = a002496_ks[n - 1];
    return (seq_t)k * (seq_t)k + 1;
}

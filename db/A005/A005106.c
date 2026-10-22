/*
 * A005106  Class 2+ primes (for definition see A005105).
 * https://oeis.org/A005106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search. class(p) = 1 + max class(q) over prime factors q >
 *            3 of p+1 (class 1 if p+1 is 3-smooth); a(n) = n-th prime p with
 *            class(p) = 2
 * Range:     u8 n <= 28, u16 n <= 836, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *res = NULL;
static idx_t rescnt = 0;
static uint64_t built_limit = 0;

static void build(uint64_t L) {
    uint64_t M = L + 2;
    uint32_t *spf = (uint32_t *)calloc(M, sizeof(uint32_t));
    unsigned char *cls = (unsigned char *)calloc(M, 1);
    for (uint64_t i = 2; i < M; i++) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j < M; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    free(res);
    res = (uint32_t *)malloc(sizeof(uint32_t) * (M / 2 + 16));
    rescnt = 0;
    for (uint64_t p = 2; p <= L; p++) {
        if (spf[p] != p) continue;
        uint64_t m = p + 1;
        int mx = 0;
        while (m > 1) {
            uint32_t q = spf[m];
            if (q > 3 && cls[q] > mx) mx = cls[q];
            while (m % q == 0) m /= q;
        }
        cls[p] = (unsigned char)(mx + 1);
        if (cls[p] == 2) res[rescnt++] = (uint32_t)p;
    }
    free(spf);
    free(cls);
    built_limit = L;
}

seq_t a(idx_t n) {
    uint64_t L = built_limit ? built_limit : (1u << 16);
    if (built_limit == 0) build(L);
    while (rescnt < n) {
        L *= 2;
        build(L);
    }
    return (seq_t)res[n - 1];
}

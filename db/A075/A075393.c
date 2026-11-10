/*
 * A075393  Positive integers k that are not divisible by A002034(k).
 * https://oeis.org/A075393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive k with A002034(k) not dividing k; computed
 *            by sieve (smallest-prime-factor sieve + Kempner numbers),
 *            doubling the limit as needed
 * Range:     u8 n <= 16, u16 n <= 874, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 998 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *A075393_terms = 0;
static size_t A075393_cnt = 0;
static unsigned long long A075393_lim = 0;

static unsigned long long A075393_kemp_pp(unsigned long long p, unsigned e) {
    unsigned long long m = 0;
    unsigned c = 0;
    while (c < e) {
        m += p;
        unsigned long long t = m;
        while (t % p == 0) { t /= p; c++; }
    }
    return m;
}

static void A075393_build(unsigned long long L) {
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint32_t *res = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)L + 1));
    unsigned long long i, j;
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i) if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    free(A075393_terms);
    A075393_terms = res;
    A075393_cnt = 0;
    for (i = 1; i <= L; i++) {
        unsigned long long t = i, s = 1;
        while (t > 1) {
            unsigned long long p = spf[t];
            unsigned e = 0;
            while (t % p == 0) { t /= p; e++; }
            unsigned long long m = (e == 1) ? p : A075393_kemp_pp(p, e);
            if (m > s) s = m;
        }
        if (i % s != 0) A075393_terms[A075393_cnt++] = (uint32_t)i;
    }
    free(spf);
    A075393_lim = L;
}

seq_t a(idx_t n) {
    if (A075393_lim == 0) A075393_build(4096);
    while ((unsigned long long)n > A075393_cnt) A075393_build(A075393_lim * 2);
    return (seq_t)A075393_terms[(size_t)(n - 1)];
}

/*
 * A005107  Class 3+ primes (for definition see A005105).
 * https://oeis.org/A005107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search. Erdos-Selfridge class: class(p) = 1 + max class(q)
 *            over primes q > 3 dividing p+1 (0 if none); a(n) = n-th prime
 *            with class(p) = 3
 * Range:     u8 n <= 11, u16 n <= 2876, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *res = NULL;
static idx_t res_len = 0;

static void build(uint64_t L) {
    uint32_t *spf = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    unsigned char *cls = (unsigned char *)calloc(L + 2, 1);
    uint64_t i, j;
    for (i = 2; i <= L + 1; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L + 1; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    free(res);
    res = NULL;
    res_len = 0;
    idx_t cap = 1024;
    res = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (i = 2; i <= L; i++) {
        if (spf[i] != i) continue;
        uint64_t m = i + 1;
        int mx = 0;
        while (m > 1) {
            uint64_t q = spf[m];
            if (q > 3) {
                int c = cls[q];
                if (c > mx) mx = c;
            }
            while (m % q == 0) m /= q;
        }
        cls[i] = (unsigned char)(mx + 1);
        if (cls[i] == 3) {
            if (res_len == cap) {
                cap *= 2;
                res = (uint64_t *)realloc(res, cap * sizeof(uint64_t));
            }
            res[res_len++] = i;
        }
    }
    free(spf);
    free(cls);
}

seq_t a(idx_t n) {
    static uint64_t L = 0;
    if (L == 0) { L = 1 << 16; build(L); }
    while (res_len < n) {
        L *= 2;
        build(L);
    }
    return (seq_t)res[n - 1];
}

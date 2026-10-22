/*
 * A001125  Primes with 6 as smallest primitive root.
 * https://oeis.org/A001125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over primes p in increasing order; a(n) is the n-th prime
 *            p > 6 such that 2, 3 and 5 are not primitive roots mod p but 6
 *            is (4 is a square, so it never is one). A smallest-prime-factor
 *            sieve that grows as needed factors p-1
 * Range:     u8 n <= 5, u16 n <= 370, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a001125_isroot, a001125_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a001125_isroot.h"
#include "../utils/a001125_pw.h"

static uint64_t *a001125_terms = NULL;
static idx_t a001125_cnt = 0;

static void a001125_build(uint64_t L) {
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint64_t i, j, p;
    idx_t cap = 1024, cnt = 0;
    uint64_t *t = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i <= L / i)
                for (j = i * i; j <= L; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (p = 7; p <= L; p += 2) {
        uint64_t qs[64], m;
        int nq = 0;
        if (spf[p] != p) continue;
        m = p - 1;
        while (m > 1) {
            uint64_t q = spf[m];
            qs[nq++] = q;
            while (m % q == 0) m /= q;
        }
        if (a001125_isroot(2, p, qs, nq)) continue;
        if (a001125_isroot(3, p, qs, nq)) continue;
        if (a001125_isroot(5, p, qs, nq)) continue;
        if (!a001125_isroot(6, p, qs, nq)) continue;
        if (cnt == cap) {
            cap *= 2;
            t = (uint64_t *)realloc(t, (size_t)cap * sizeof(uint64_t));
        }
        t[cnt++] = p;
    }
    free(spf);
    free(a001125_terms);
    a001125_terms = t;
    a001125_cnt = cnt;
}

seq_t a(idx_t n) {
    static uint64_t L = 0;
    if (n < 1) return 0;
    while (a001125_cnt < n) {
        L = L ? L * 2 : (uint64_t)1 << 17;
        a001125_build(L);
    }
    return (seq_t)a001125_terms[n - 1];
}

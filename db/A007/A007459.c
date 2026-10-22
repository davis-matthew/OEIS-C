/*
 * A007459  Higgs's primes: a(n+1) = smallest prime > a(n) such that a(n+1)-1
 *          divides the product (a(1)...a(n))^2.
 * https://oeis.org/A007459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   sieve search. A prime p is a term iff every prime factor q of
 *            p-1 is an earlier term and q^2 is the highest power of q that
 *            may divide p-1 (a(1) = 2). Smallest-prime-factor sieve, doubling
 *            the limit as needed
 * Range:     u8 n <= 35, u16 n <= 2227, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *higgs_terms = NULL;
static idx_t higgs_count = 0;

static void higgs_compute(uint64_t L) {
    uint32_t *spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    bool *ish = (bool *)calloc((size_t)(L + 1), sizeof(bool));
    uint64_t i, j, cap = 1024, cnt = 0;
    uint64_t *t = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i <= L / i)
                for (j = i * i; j <= L; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (i = 2; i <= L; i++) {
        if (spf[i] != i) continue;
        uint64_t m = i - 1;
        bool ok = true;
        while (m > 1 && ok) {
            uint64_t q = spf[m];
            int e = 0;
            while (m % q == 0) { m /= q; e++; }
            if (e > 2 || !ish[q]) ok = false;
        }
        if (ok) {
            ish[i] = true;
            if (cnt == cap) {
                cap *= 2;
                t = (uint64_t *)realloc(t, (size_t)cap * sizeof(uint64_t));
            }
            t[cnt++] = i;
        }
    }
    free(spf);
    free(ish);
    if (higgs_terms) free(higgs_terms);
    higgs_terms = t;
    higgs_count = (idx_t)cnt;
}

seq_t a(idx_t n) {
    static uint64_t L = 0;
    if (n < 1) return (seq_t)0;
    while (higgs_count < n) {
        L = (L == 0) ? ((uint64_t)1 << 16) : L * 2;
        higgs_compute(L);
    }
    return (seq_t)higgs_terms[n - 1];
}

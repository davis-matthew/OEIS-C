/*
 * A005114  Untouchable numbers, also called nonaliquot numbers: impossible
 *          values for the sum of aliquot parts function (A001065).
 * https://oeis.org/A005114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   sieve. a(n) is the n-th k >= 1 with no m such that sigma(m) - m
 *            = k. For m = P^e*j, with P the largest prime factor of m, s(m)
 *            >= m/P. So every m with s(m) <= L can be listed from j <= L and
 *            primes P <= L
 * Range:     u8 n <= 16, u16 n <= 8941, u32 n <= 9761, u64 n <= 9761,
 *            u128 n <= 9761, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *res = NULL;
static idx_t res_count = 0;
static uint64_t res_limit = 0;

static void build(uint64_t L) {
    uint64_t *sig = (uint64_t *)calloc((size_t)(L + 1), sizeof(uint64_t));
    uint32_t *gpf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    unsigned char *touched = (unsigned char *)calloc((size_t)(L + 1), 1);
    uint32_t *primes = (uint32_t *)malloc((size_t)(L + 1) * sizeof(uint32_t));
    uint64_t np = 0, i, k;
    for (i = 1; i <= L; i++)
        for (k = i; k <= L; k += i) sig[k] += i;
    gpf[1] = 1;
    for (i = 2; i <= L; i++) {
        if (gpf[i] == 0) {
            primes[np++] = (uint32_t)i;
            for (k = i; k <= L; k += i) gpf[k] = (uint32_t)i;
        }
    }
    if (L >= 1) touched[1] = 1; /* primes */
    for (uint64_t j = 1; j <= L; j++) {
        uint64_t g = gpf[j];
        /* first prime index with prime > g */
        uint64_t lo = 0, hi = np;
        while (lo < hi) {
            uint64_t mid = (lo + hi) / 2;
            if (primes[mid] <= g) lo = mid + 1; else hi = mid;
        }
        uint64_t start = lo;
        /* case e = 1, j >= 2: s = P*s(j) + sigma(j) */
        if (j >= 2 && sig[j] < L) {
            uint64_t sj = sig[j] - j;
            for (uint64_t t = start; t < np; t++) {
                uint64_t P = primes[t];
                uint64_t s = P * sj + sig[j];
                if (s > L) break;
                touched[s] = 1;
            }
        }
        /* case e >= 2: need P^(e-1)*j <= L */
        for (uint64_t t = start; t < np; t++) {
            uint64_t P = primes[t];
            if (P * j > L) break;
            uint64_t pprev = P;          /* P^(e-1) */
            uint64_t pe = P * P;         /* P^e */
            while (pprev * j <= L) {
                uint64_t sp = (pe * P - 1) / (P - 1);
                uint64_t s = sp * sig[j] - pe * j;
                if (s <= L) touched[s] = 1;
                pprev = pe;
                pe *= P;
            }
        }
    }
    free(res);
    res = (uint64_t *)malloc((size_t)(L + 1) * sizeof(uint64_t));
    res_count = 0;
    for (i = 1; i <= L; i++)
        if (!touched[i]) res[res_count++] = i;
    res_limit = L;
    free(sig); free(gpf); free(touched); free(primes);
}

seq_t a(idx_t n) {
    uint64_t L = res_limit ? res_limit : 1024;
    if (res == NULL) build(L);
    while (res_count < n) {
        L *= 2;
        build(L);
    }
    return (seq_t)res[n - 1];
}

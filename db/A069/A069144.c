/*
 * A069144  Numbers k such that A068976(k) divides k.
 * https://oeis.org/A069144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th k with A068976(k) | k, where A068976(k) = Sum_{d|k}
 *            gcd(d,k/d)^2 (multiplicative); found by a sieve with doubling
 *            limit
 * Range:     u8 n <= 6, u16 n <= 421, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9697 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A069144_terms = 0;
static size_t A069144_cnt = 0;
static uint64_t A069144_limit = 0;

static void A069144_build(uint64_t L)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint64_t *f = (uint64_t *)malloc(((size_t)L + 1) * sizeof(uint64_t));
    uint64_t *t = (uint64_t *)malloc(((size_t)L + 1) * sizeof(uint64_t));
    size_t c = 0;
    uint64_t i, j;
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    f[0] = 0;
    if (L >= 1) f[1] = 1;
    for (i = 1; i <= L; i++) {
        if (i > 1) {
            uint64_t p = spf[i], m = i;
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            uint64_t g = 0;
            int q;
            for (q = 0; q <= e; q++) {
                int mn = q < e - q ? q : e - q;
                uint64_t pw = 1;
                int z;
                for (z = 0; z < 2 * mn; z++) pw *= p;
                g += pw;
            }
            f[i] = f[m] * g;
        }
        if (i % f[i] == 0) t[c++] = i;
    }
    free(spf);
    free(f);
    free(A069144_terms);
    A069144_terms = t;
    A069144_cnt = c;
    A069144_limit = L;
}

seq_t a(idx_t n)
{
    if (n < 1) n = 1;
    if (A069144_limit == 0) A069144_build(4096);
    while (A069144_cnt < (size_t)n) A069144_build(A069144_limit * 2);
    return (seq_t)A069144_terms[n - 1];
}

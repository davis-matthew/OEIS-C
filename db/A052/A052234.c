/*
 * A052234  Primes p from A031924 such that A052180(primepi(p)) = 17.
 * https://oeis.org/A052234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p with prime(k+1)-p = 6 (p in A031924) and
 *            A052180(k) = 17, where p = prime(k); found by an spf sieve with
 *            a doubling limit
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A052234_terms = NULL;
static size_t A052234_cnt = 0;

static void A052234_build(uint64_t limit)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    uint64_t *out = NULL;
    size_t cnt = 0, cap = 0;
    uint64_t i, j;
    if (!spf) abort();
    for (i = 2; i <= limit; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= limit)
                for (j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    uint64_t prev = 0;
    for (i = 2; i <= limit; i++) {
        if (spf[i] != i) continue;
        if (prev && i - prev == 6) {
            uint32_t mx = 0;
            for (j = prev + 1; j < i; j++) {
                if (spf[j] > mx) mx = spf[j];
            }
            if (mx == 17) {
                if (cnt == cap) {
                    cap = cap ? cap * 2 : 64;
                    uint64_t *t = (uint64_t *)realloc((void *)out, cap * sizeof(uint64_t));
                    if (!t) abort();
                    out = t;
                }
                out[cnt++] = prev;
            }
        }
        prev = i;
    }
    free(spf);
    free(A052234_terms);
    A052234_terms = out;
    A052234_cnt = cnt;
}

seq_t a(idx_t n)
{
    static uint64_t limit = 0;
    if (n < 1) return 0;
    while (A052234_cnt < (size_t)n) {
        limit = limit ? limit * 4 : (1u << 16);
        A052234_build(limit);
    }
    return (seq_t)A052234_terms[n - 1];
}

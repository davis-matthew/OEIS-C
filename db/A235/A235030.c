/*
 * A235030  Numbers such that A235027(A235027(n)) <> n; Numbers which are
 *          divisible by any of the odd terms of A204219.
 * https://oeis.org/A235030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer divisible by some odd prime p whose
 *            binary reversal is not prime (A204219 after 2); computed by a
 *            doubling sieve
 * Range:     u8 n <= 35, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 70 terms through n = 9605 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t *A235030_terms = NULL;
static uint64_t A235030_count = 0;

static void A235030_build(uint64_t L)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)L + 1, 1);
    unsigned char *mark = (unsigned char *)calloc((size_t)L + 1, 1);
    uint64_t p, m, cnt = 0;
    for (p = 2; p <= L; p++) {
        if (comp[p]) continue;
        for (m = p * p; m <= L && p * p <= L; m += p) comp[m] = 1;
        if (p == 2) continue;
        {
            uint64_t x = p, r = 0;
            while (x) { r = (r << 1) | (x & 1); x >>= 1; }
            if (!is_prime_u64(r)) {
                for (m = p; m <= L; m += p) mark[m] = 1;
            }
        }
    }
    for (m = 1; m <= L; m++) if (mark[m]) cnt++;
    free(A235030_terms);
    A235030_terms = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    cnt = 0;
    for (m = 1; m <= L; m++) if (mark[m]) A235030_terms[cnt++] = m;
    A235030_count = cnt;
    free(comp);
    free(mark);
}

seq_t a(idx_t n)
{
    static uint64_t L = 0;
    if (n == 0) return (seq_t)0;
    while (A235030_count < (uint64_t)n) {
        L = L ? L * 2 : 1024;
        A235030_build(L);
    }
    return (seq_t)A235030_terms[(size_t)(n - 1)];
}

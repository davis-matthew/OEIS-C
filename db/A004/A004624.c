/*
 * A004624  Divisible only by primes congruent to 6 mod 7.
 * https://oeis.org/A004624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search — a(n) is the n-th positive integer whose prime
 *            factors p all satisfy p == 6 (mod 7) (a(1) = 1)
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *terms = NULL;
static idx_t nterms = 0;
static uint64_t limit = 0;

static void build(uint64_t L) {
    uint32_t *spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint64_t i, j, cap = 1024, cnt = 0;
    uint64_t *t = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (i = 1; i <= L; i++) {
        uint64_t m = i;
        bool ok = true;
        while (m > 1) {
            uint64_t p = spf[m];
            if (p % 7 != 6) { ok = false; break; }
            m /= p;
        }
        if (ok) {
            if (cnt == cap) {
                cap *= 2;
                t = (uint64_t *)realloc(t, (size_t)cap * sizeof(uint64_t));
            }
            t[cnt++] = i;
        }
    }
    free(spf);
    if (terms) free(terms);
    terms = t;
    nterms = (idx_t)cnt;
    limit = L;
}

seq_t a(idx_t n) {
    if (limit == 0) build(1024);
    while (nterms < n) build(limit * 2);
    return (seq_t)terms[n - 1];
}

/*
 * A004627  Divisible only by primes congruent to 5 mod 8.
 * https://oeis.org/A004627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve of smallest prime factors; a(n) is the n-th m >= 1 whose
 *            prime factors are all congruent to 5 mod 8 (a(1) = 1)
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *terms = NULL;
static idx_t nterms = 0;
static uint64_t sieved_limit = 0;

static void build(uint64_t limit) {
    uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    uint64_t i, j, cap, m;
    idx_t cnt = 0;
    uint64_t *t;
    for (i = 2; i <= limit; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    cap = 1024;
    t = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
    for (m = 1; m <= limit; m += 2) {
        uint64_t x = m;
        bool ok = true;
        if (m % 4 != 1) continue;
        while (x > 1) {
            uint32_t p = spf[x];
            if (p % 8 != 5) { ok = false; break; }
            x /= p;
        }
        if (!ok) continue;
        if (cnt == cap) {
            cap *= 2;
            t = (uint64_t *)realloc(t, (size_t)cap * sizeof(uint64_t));
        }
        t[cnt++] = m;
    }
    free(spf);
    free(terms);
    terms = t;
    nterms = cnt;
    sieved_limit = limit;
}

seq_t a(idx_t n) {
    uint64_t limit = sieved_limit ? sieved_limit : ((uint64_t)1 << 16);
    if (terms == NULL) build(limit);
    while (nterms < n) {
        limit = sieved_limit * 2;
        build(limit);
    }
    return (seq_t)terms[n - 1];
}

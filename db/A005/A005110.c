/*
 * A005110  Class 2- primes (for definition see A005109).
 * https://oeis.org/A005110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search. Class of prime p: 1 if p-1 has no prime factors
 *            other than 2 and 3, otherwise 1 + max class of the prime factors
 *            q > 3 of p-1. a(n) = n-th prime of class 2
 * Range:     u8 n <= 27, u16 n <= 720, u32 n <= 9875, u64 n <= 9875,
 *            u128 n <= 9875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static uint64_t *a5110_terms = NULL;
static size_t a5110_count = 0;

static void a5110_build(uint32_t limit) {
    uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    unsigned char *cls = (unsigned char *)calloc((size_t)limit + 1, 1);
    uint64_t *out = (uint64_t *)malloc(((size_t)limit / 2 + 2) * sizeof(uint64_t));
    size_t cnt = 0;
    uint32_t i, j;
    for (i = 2; i <= limit; i++) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * i <= limit)
                for (j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
        }
    }
    for (i = 2; i <= limit; i++) {
        if (spf[i] != i) continue;
        {
            uint32_t m = i - 1;
            unsigned char mx = 0;
            while (m > 1) {
                uint32_t q = spf[m];
                while (m % q == 0) m /= q;
                if (q > 3 && cls[q] > mx) mx = cls[q];
            }
            cls[i] = (unsigned char)(mx + 1);
            if (cls[i] == 2) out[cnt++] = i;
        }
    }
    free(spf);
    free(cls);
    free(a5110_terms);
    a5110_terms = out;
    a5110_count = cnt;
}

seq_t a(idx_t n) {
    uint32_t limit = 1u << 16;
    while (a5110_count < n) {
        a5110_build(limit);
        if (a5110_count >= n) break;
        limit *= 2;
    }
    return (seq_t)a5110_terms[n - 1];
}

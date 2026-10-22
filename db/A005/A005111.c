/*
 * A005111  Class 3- primes (for definition see A005109).
 * https://oeis.org/A005111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve; Erdos-Selfridge class of prime p is 1 + max class of the
 *            prime factors of p-1, with 2 and 3 counting as class 0; list
 *            primes of class 3
 * Range:     u8 n <= 10, u16 n <= 2463, u32 n <= 9260, u64 n <= 9260,
 *            u128 n <= 9260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *found = NULL;
static idx_t nfound = 0;
static uint64_t sieved_limit = 0;

static void build(uint64_t limit) {
    uint32_t *spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    unsigned char *cls = (unsigned char *)calloc(limit + 1, 1);
    uint64_t i, j;
    idx_t cap = 1024;
    free(found);
    found = (uint64_t *)malloc(cap * sizeof(uint64_t));
    nfound = 0;
    for (i = 2; i <= limit; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= limit)
                for (j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (i = 2; i <= limit; i++) {
        if (spf[i] != i) continue;
        if (i <= 3) { cls[i] = 0; continue; }
        {
            uint64_t m = i - 1;
            unsigned char mx = 0;
            while (m > 1) {
                uint32_t q = spf[m];
                if (cls[q] > mx) mx = cls[q];
                while (m % q == 0) m /= q;
            }
            cls[i] = (unsigned char)(mx + 1);
        }
        if (cls[i] == 3) {
            if (nfound == cap) {
                cap *= 2;
                found = (uint64_t *)realloc(found, cap * sizeof(uint64_t));
            }
            found[nfound++] = i;
        }
    }
    free(spf);
    free(cls);
    sieved_limit = limit;
}

seq_t a(idx_t n) {
    uint64_t limit;
    if (n < 1) return 0;
    limit = sieved_limit ? sieved_limit : 4096;
    while (nfound < n) {
        if (sieved_limit) limit *= 2;
        build(limit);
    }
    return (seq_t)found[n - 1];
}

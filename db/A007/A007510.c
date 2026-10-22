/*
 * A007510  Single (or isolated or non-twin) primes: Primes p such that
 *          neither p-2 nor p+2 is prime.
 * https://oeis.org/A007510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   sieve of Eratosthenes; a(n) is the n-th prime p such that
 *            neither p-2 nor p+2 is prime
 * Range:     u8 n <= 21, u16 n <= 4823, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *singles = NULL;
static idx_t singles_len = 0;
static uint64_t sieved_limit = 0;

static void build(uint64_t limit) {
    uint64_t sz = limit + 3;
    unsigned char *comp = (unsigned char *)malloc((size_t)sz);
    uint64_t i, j, cnt = 0;
    memset(comp, 0, (size_t)sz);
    comp[0] = 1;
    comp[1] = 1;
    for (i = 2; i * i < sz; i++) {
        if (!comp[i]) {
            for (j = i * i; j < sz; j += i) comp[j] = 1;
        }
    }
    for (i = 2; i <= limit; i++) {
        if (!comp[i] && (i < 2 || i - 2 < 2 || comp[i - 2]) && comp[i + 2]) {
            if (i >= 4 && !comp[i - 2]) continue;
            cnt++;
        }
    }
    free(singles);
    singles = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    singles_len = 0;
    for (i = 2; i <= limit; i++) {
        if (comp[i]) continue;
        if (i >= 4 && !comp[i - 2]) continue;
        if (!comp[i + 2]) continue;
        singles[singles_len++] = i;
    }
    sieved_limit = limit;
    free(comp);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n > singles_len) {
        uint64_t limit = sieved_limit ? sieved_limit : 1024;
        while (1) {
            if (limit > sieved_limit) build(limit);
            if (n <= singles_len) break;
            limit *= 2;
        }
    }
    return (seq_t)singles[n - 1];
}

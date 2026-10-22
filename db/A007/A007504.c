/*
 * A007504  Sum of the first n primes.
 * https://oeis.org/A007504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=1..n} prime(k), a(0) = 0; primes come from a
 *            cached sieve of Eratosthenes, doubled in size as needed
 * Range:     u8 n <= 13, u16 n <= 157, u32 n <= 9305, u64 n <= 9305,
 *            u128 n <= 9305, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *a7504_primes = NULL;
static idx_t a7504_count = 0;
static uint64_t a7504_limit = 0;

static void a7504_sieve(uint64_t limit) {
    unsigned char *comp = (unsigned char *)malloc((size_t)(limit + 1));
    uint64_t i, j;
    idx_t cnt = 0;
    memset(comp, 0, (size_t)(limit + 1));
    for (i = 2; i * i <= limit; i++) {
        if (!comp[i]) {
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
        }
    }
    for (i = 2; i <= limit; i++) if (!comp[i]) cnt++;
    free(a7504_primes);
    a7504_primes = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    cnt = 0;
    for (i = 2; i <= limit; i++) if (!comp[i]) a7504_primes[cnt++] = i;
    free(comp);
    a7504_count = cnt;
    a7504_limit = limit;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t k;
    if (n == 0) return sum;
    while (a7504_count < n) {
        uint64_t lim = a7504_limit ? a7504_limit * 2 : 1024;
        a7504_sieve(lim);
    }
    for (k = 0; k < n; k++) sum += (seq_t)a7504_primes[k];
    return sum;
}

/*
 * A001348  Mersenne numbers: 2^p - 1, where p is prime.
 * https://oeis.org/A001348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2^prime(n) - 1, where prime(n) = A000040(n) is found with
 *            a sieve
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   a1348_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/a1348_pow2.h"

static uint64_t *a1348_primes = NULL;
static uint64_t a1348_count = 0;

static void a1348_sieve(uint64_t limit) {
    unsigned char *comp = (unsigned char *)malloc((size_t)(limit + 1));
    uint64_t i, j, cnt = 0;
    memset(comp, 0, (size_t)(limit + 1));
    for (i = 2; i * i <= limit; i++)
        if (!comp[i])
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
    for (i = 2; i <= limit; i++) if (!comp[i]) cnt++;
    free(a1348_primes);
    a1348_primes = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    cnt = 0;
    for (i = 2; i <= limit; i++) if (!comp[i]) a1348_primes[cnt++] = i;
    a1348_count = cnt;
    free(comp);
}

static uint64_t a1348_prime(idx_t n) {
    uint64_t limit = 1024;
    while (a1348_count < n) {
        a1348_sieve(limit);
        limit *= 2;
    }
    return a1348_primes[n - 1];
}

seq_t a(idx_t n) {
    uint64_t p = a1348_prime(n);
    return a1348_pow2(p) - 1;
}

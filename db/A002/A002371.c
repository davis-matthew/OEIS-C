/*
 * A002371  Period of decimal expansion of 1/(n-th prime) (0 by convention for
 *          the primes 2 and 5).
 * https://oeis.org/A002371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, base
 * Formula:   a(n) = multiplicative order of 10 mod prime(n), i.e. least m
 *            with 10^m == 1 (mod prime(n)); a(n) = 0 for prime(n) = 2, 5.
 *            Primes found by a sieve
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 9274, u64 n <= 9274,
 *            u128 n <= 9274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a2371_primes = NULL;
static uint64_t a2371_count = 0;

static void a2371_sieve(uint64_t limit) {
    unsigned char *comp = (unsigned char *)malloc((size_t)(limit + 1));
    uint64_t i, j, cnt = 0;
    memset(comp, 0, (size_t)(limit + 1));
    for (i = 2; i <= limit; i++) {
        if (!comp[i]) {
            cnt++;
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
        }
    }
    free(a2371_primes);
    a2371_primes = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (i = 2; i <= limit; i++)
        if (!comp[i]) a2371_primes[cnt++] = i;
    a2371_count = cnt;
    free(comp);
}

static uint64_t a2371_nth_prime(idx_t n) {
    uint64_t limit = 1024;
    while (a2371_count < n) {
        while (limit < 64 || limit / 2 < (uint64_t)n * 8) limit *= 2;
        a2371_sieve(limit);
        limit *= 2;
    }
    return a2371_primes[n - 1];
}

seq_t a(idx_t n) {
    uint64_t p = a2371_nth_prime(n);
    uint64_t r;
    seq_t m;

    if (p == 2 || p == 5) return 0;

    m = 1;
    r = 10 % p;
    while (r != 1) {
        r = (r * 10) % p;
        m++;
    }
    return m;
}

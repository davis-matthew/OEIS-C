/*
 * A001750  Primes multiplied by 5.
 * https://oeis.org/A001750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*prime(n); primes found with a sieve of Eratosthenes
 * Range:     u8 n <= 15, u16 n <= 1559, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a001750_primes = NULL;
static uint64_t a001750_count = 0;
static uint64_t a001750_limit = 0;

static void a001750_sieve(uint64_t limit) {
    unsigned char *comp = (unsigned char *)malloc((size_t)(limit + 1));
    uint64_t i, j, cnt = 0;
    if (!comp) return;
    memset(comp, 0, (size_t)(limit + 1));
    for (i = 2; i * i <= limit; i++)
        if (!comp[i])
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
    for (i = 2; i <= limit; i++) if (!comp[i]) cnt++;
    uint64_t *pr = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    if (!pr) { free(comp); return; }
    cnt = 0;
    for (i = 2; i <= limit; i++) if (!comp[i]) pr[cnt++] = i;
    free(comp);
    free(a001750_primes);
    a001750_primes = pr;
    a001750_count = cnt;
    a001750_limit = limit;
}

seq_t a(idx_t n) {
    uint64_t lim = a001750_limit ? a001750_limit : 1024;
    while (a001750_count < n) {
        if (a001750_limit >= lim) lim *= 2;
        a001750_sieve(lim);
    }
    return (seq_t)a001750_primes[n - 1] * 5;
}

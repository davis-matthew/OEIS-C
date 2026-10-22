/*
 * A001747  2 together with primes multiplied by 2.
 * https://oeis.org/A001747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1) = 2; a(n) = 2*prime(n-1) for n >= 2 (primes found by a
 *            sieve of Eratosthenes, enlarged by doubling as needed)
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a001747_primes = NULL;
static idx_t a001747_count = 0;

static void a001747_sieve(uint64_t limit) {
    unsigned char *comp = (unsigned char *)malloc((size_t)limit + 1);
    uint64_t i, j;
    idx_t cnt = 0;
    memset(comp, 0, (size_t)limit + 1);
    for (i = 2; i * i <= limit; i++) {
        if (!comp[i]) {
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
        }
    }
    for (i = 2; i <= limit; i++) if (!comp[i]) cnt++;
    free(a001747_primes);
    a001747_primes = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (i = 2; i <= limit; i++) if (!comp[i]) a001747_primes[cnt++] = i;
    a001747_count = cnt;
    free(comp);
}

seq_t a(idx_t n) {
    idx_t k;
    uint64_t limit = 1024;
    if (n <= 1) return (seq_t)2;
    k = n - 1;
    while (a001747_count < k) {
        a001747_sieve(limit);
        limit *= 2;
    }
    return (seq_t)2 * (seq_t)a001747_primes[k - 1];
}

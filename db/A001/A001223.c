/*
 * A001223  Prime gaps: differences between consecutive primes.
 * https://oeis.org/A001223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, hear, changed
 * Formula:   a(n) = prime(n+1) - prime(n); primes from a sieve of
 *            Eratosthenes that doubles its limit as needed
 * Range:     u8 n <= 9733, u16 n <= 9733, u32 n <= 9733, u64 n <= 9733,
 *            u128 n <= 9733, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a001223_primes = NULL;
static size_t a001223_count = 0;
static uint64_t a001223_limit = 0;

static void a001223_build(size_t need)
{
    uint64_t lim = a001223_limit ? a001223_limit : 1024;
    while (a001223_count < need) {
        if (a001223_limit) lim = a001223_limit * 2;
        unsigned char *comp = (unsigned char *)malloc((size_t)lim + 1);
        if (!comp) return;
        memset(comp, 0, (size_t)lim + 1);
        size_t cnt = 0;
        uint64_t i, j;
        for (i = 2; i <= lim; i++) {
            if (!comp[i]) {
                cnt++;
                if (i * i <= lim)
                    for (j = i * i; j <= lim; j += i) comp[j] = 1;
            }
        }
        uint64_t *arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        if (!arr) { free(comp); return; }
        size_t k = 0;
        for (i = 2; i <= lim; i++)
            if (!comp[i]) arr[k++] = i;
        free(comp);
        free(a001223_primes);
        a001223_primes = arr;
        a001223_count = cnt;
        a001223_limit = lim;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    size_t need = (size_t)n + 1;
    if (a001223_count < need) a001223_build(need);
    uint64_t gap = a001223_primes[n] - a001223_primes[n - 1];
    return (seq_t)gap;
}

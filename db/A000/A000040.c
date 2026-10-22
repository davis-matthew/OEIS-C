/*
 * A000040  The prime numbers.
 * https://oeis.org/A000040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  core, nonn, nice, easy, changed
 * Formula:   a(n) = n-th prime; computed by a sieve of Eratosthenes, doubled
 *            in size until at least n primes are found
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a000040_primes = NULL;
static size_t a000040_count = 0;
static uint64_t a000040_limit = 0;

static void a000040_ensure(idx_t n)
{
    while ((idx_t)a000040_count < n) {
        uint64_t lim = a000040_limit < 64 ? 64 : a000040_limit * 2;
        unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
        uint64_t i, j;
        size_t cnt = 0;
        uint64_t *pr;
        if (!comp)
            return;
        for (i = 2; i * i <= lim; i++)
            if (!comp[i])
                for (j = i * i; j <= lim; j += i)
                    comp[j] = 1;
        for (i = 2; i <= lim; i++)
            if (!comp[i])
                cnt++;
        pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        if (!pr) {
            free(comp);
            return;
        }
        cnt = 0;
        for (i = 2; i <= lim; i++)
            if (!comp[i])
                pr[cnt++] = i;
        free(comp);
        free(a000040_primes);
        a000040_primes = pr;
        a000040_count = cnt;
        a000040_limit = lim;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    a000040_ensure(n);
    if ((idx_t)a000040_count < n)
        return (seq_t)0;
    return (seq_t)a000040_primes[n - 1];
}

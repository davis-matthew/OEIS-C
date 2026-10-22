/*
 * A001043  Numbers that are the sum of 2 successive primes.
 * https://oeis.org/A001043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = prime(n) + prime(n+1); primes found with a sieve of
 *            Eratosthenes that doubles its limit as needed
 * Range:     u8 n <= 30, u16 n <= 3512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *a001043_primes = NULL;
static idx_t a001043_count = 0;
static uint64_t a001043_limit = 0;

static void a001043_sieve(uint64_t limit)
{
    unsigned char *comp = (unsigned char *)malloc((size_t)limit + 1);
    uint64_t i, j;
    idx_t cnt = 0;
    memset(comp, 0, (size_t)limit + 1);
    for (i = 2; i * i <= limit; i++)
        if (!comp[i])
            for (j = i * i; j <= limit; j += i)
                comp[j] = 1;
    for (i = 2; i <= limit; i++)
        if (!comp[i])
            cnt++;
    free(a001043_primes);
    a001043_primes = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    cnt = 0;
    for (i = 2; i <= limit; i++)
        if (!comp[i])
            a001043_primes[cnt++] = i;
    free(comp);
    a001043_count = cnt;
    a001043_limit = limit;
}

static uint64_t a001043_nth_prime(idx_t k)
{
    uint64_t limit = a001043_limit ? a001043_limit : 1024;
    while (a001043_count < k) {
        if (a001043_limit)
            limit *= 2;
        a001043_sieve(limit);
    }
    return a001043_primes[k - 1];
}

seq_t a(idx_t n)
{
    uint64_t p1 = a001043_nth_prime(n);
    uint64_t p2 = a001043_nth_prime(n + 1);
    return (seq_t)p1 + (seq_t)p2;
}

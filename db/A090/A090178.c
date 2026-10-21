/*
 * A090178  a(1) = 2; for n > 1, a(n) = n + prime(n-1).
 * https://oeis.org/A090178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + prime(n-1)
 * Range:     u8 n <= 47, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 2; for n > 1, a(n) = n + the (n-1)-th prime number. */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stddef.h>

/* Static storage for primes. */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t prime_count = 0;        /* number of primes stored */

/* Extend the prime list so that it contains at least `need` primes. */
static void build_primes(size_t need)
{
    if (prime_count >= need)
        return;                         /* already have enough */

    size_t est_limit;
    if (need < 6)                      /* small case */
        est_limit = 15;
    else {
        double n = (double)need;
        est_limit = (size_t)(n * (log(n) + log(log(n)))) + 3;
    }

    /* Allocate sieve array. */
    uint8_t *sieve = (uint8_t *)malloc(est_limit + 1);
    if (!sieve)
        abort();                       /* out of memory */
    memset(sieve, 0, est_limit + 1);

    /* Simple Eratosthenes sieve. */
    for (size_t i = 2; i * i <= est_limit; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= est_limit; j += i)
                sieve[j] = 1;
        }
    }

    /* Resize prime array to hold `need` primes. */
    uint64_t *new_list = (uint64_t *)realloc(prime_list, need * sizeof(uint64_t));
    if (!new_list)
        abort();
    prime_list = new_list;

    /* Fill prime array with primes found up to est_limit. */
    size_t idx = 0;
    for (size_t i = 2; i <= est_limit && idx < need; ++i) {
        if (!sieve[i])
            prime_list[idx++] = (uint64_t)i;
    }
    prime_count = idx;

    free(sieve);
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)2;                /* special case */

    size_t needed = (size_t)(n - (idx_t)1);   /* (n-1)-th prime needed */
    build_primes(needed);

    /* n is at least 2 here, so needed >= 1 and prime_list[needed-1] exists */
    seq_t result = (seq_t)n + (seq_t)prime_list[needed - 1];
    return result;
}

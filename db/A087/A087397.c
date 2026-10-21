/*
 * A087397  Smallest triangular number > 1 and == 1 (mod prime(n)).
 * https://oeis.org/A087397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-2)*(prime(n)-1)/2 for n ≥ 3, a(1) = 3, a(2) =
 *            10 The function generates the first n primes lazily using a
 *            simple Eratosthenes sieve bounded by an upper estimate for the
 *            nth prime
 * Range:     u8 n <= 9, u16 n <= 72, u32 n <= 6627, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

static uint32_t *prime_arr = NULL;      /* array of primes */
static size_t prime_count = 0;          /* number of primes stored */
static size_t prime_arr_size = 0;       /* allocated capacity */

/* estimate an upper bound for the n-th prime */
static uint32_t estimate_prime_upper(idx_t n)
{
    if (n < 6) return 15;
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn)));
    return (uint32_t)(bound + 3);   /* add a small safety margin */
}

/* produce at least n primes, storing them in prime_arr */
static void generate_primes(idx_t n)
{
    if (prime_count >= (size_t)n) return;  /* already have enough */

    uint32_t limit = estimate_prime_upper(n);

    /* allocate enough room for primes */
    if (prime_arr_size < (size_t)(limit/2 + 10)) {
        free(prime_arr);
        prime_arr = (uint32_t*)malloc(sizeof(uint32_t) * (size_t)(limit/2 + 10));
        prime_arr_size = (size_t)(limit/2 + 10);
    }

    /* sieve of Eratosthenes */
    uint8_t *is_composite = (uint8_t*)calloc((size_t)limit + 1, sizeof(uint8_t));
    size_t count = 0;
    for (uint32_t i = 2; i <= limit && count < n; ++i) {
        if (!is_composite[i]) {
            prime_arr[count++] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint32_t j = i * i; j <= limit; j += i)
                    is_composite[j] = 1;
            }
        }
    }
    free(is_composite);
    prime_count = count;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)10;

    generate_primes(n);

    seq_t p = (seq_t)prime_arr[n - 1];
    return (p - 2) * (p - 1) / 2;
}

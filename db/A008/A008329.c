/*
 * A008329  Number of divisors of p+1, p prime.
 * https://oeis.org/A008329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(A008864(n)) i.e. the number of divisors of the
 *            n-th prime plus 1
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *prime_cache = NULL;
static size_t prime_count = 0;

/* Ensure prime_cache contains at least first n primes.
   The n-th prime < n*20 for n>=6, otherwise a small constant bound is used. */
static void ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;

    idx_t limit = n < 6 ? 15 : n * 20;

    while (prime_count < (size_t)n) {
        size_t sz = (size_t)limit + 1;
        unsigned char *is_comp = (unsigned char *)calloc(sz, 1);
        if (!is_comp)
            exit(EXIT_FAILURE);

        size_t temp_cap = limit / 10 + 1;
        uint64_t *temp = (uint64_t *)malloc(temp_cap * sizeof(uint64_t));
        if (!temp)
            exit(EXIT_FAILURE);

        size_t temp_cnt = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (!is_comp[(size_t)i]) {
                if (temp_cnt == temp_cap) {
                    temp_cap *= 2;
                    uint64_t *realloc_ptr = (uint64_t *)realloc(temp, temp_cap * sizeof(uint64_t));
                    if (!realloc_ptr)
                        exit(EXIT_FAILURE);
                    temp = realloc_ptr;
                }
                temp[temp_cnt++] = (uint64_t)i;

                if (i * i <= limit) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        is_comp[(size_t)j] = 1;
                }
            }
        }

        free(is_comp);

        /* Replace old cache with newly built one. */
        free(prime_cache);
        prime_cache   = temp;
        prime_count   = temp_cnt;
        /* prime_capacity not needed further */

        if (prime_count < (size_t)n)
            limit = n * 20; /* increase bound and repeat */
    }
}

/* Count divisors of a 64‑bit integer using the cached primes. */
static seq_t count_divisors_u64(uint64_t m)
{
    seq_t result = 1;
    for (size_t i = 0; i < prime_count && prime_cache[i] * prime_cache[i] <= m; ++i) {
        uint64_t p   = prime_cache[i];
        unsigned exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        result *= (seq_t)(exp + 1);
    }
    if (m > 1)          /* the remaining factor is prime */
        result *= 2;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)         /* undefined for offset 1, guard for safety */
        return 0;

    ensure_primes(n);
    uint64_t p = prime_cache[n - 1];   /* n-th prime, zero‑based indexing */
    uint64_t m = p + 1;
    return count_divisors_u64(m);
}

/*
 * A106868  Primes of the form 2x^2-xy+3y^2, with x and y nonnegative.
 * https://oeis.org/A106868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A106868: primes of the form 2*x^2 - x*y + 3*y^2 (x, y ≥ 0)
   The function builds a sorted list of such primes up to a fixed
   sieve limit the first time it is called, then returns the nth element
   (1‑based index). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Choose a sieve limit that will contain many terms (≈10 000th prime) */
static const uint64_t SIEVE_LIMIT = 50000000ULL;

static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va > vb) - (va < vb);
}

seq_t a(idx_t n)
{
    /* static storage for primes of the required quadratic form */
    static seq_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static size_t prime_cap = 0;
    static bool initialized = false;

    if (!initialized)
    {
        /* 1. Sieve primality up to SIEVE_LIMIT */
        unsigned char *is_prime =
            (unsigned char *)calloc(SIEVE_LIMIT + 1, sizeof(unsigned char));
        if (!is_prime)
            exit(1);
        for (uint64_t i = 0; i <= SIEVE_LIMIT; ++i)
            is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;
        for (uint64_t i = 2; i * i <= SIEVE_LIMIT; ++i)
            if (is_prime[i])
                for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_prime[j] = 0;

        /* 2. Generate primes of the form 2*x^2 - x*y + 3*y^2 */
        /* Compute y_max such that 3*y^2 <= SIEVE_LIMIT */
        uint64_t y_max = 0;
        while (3 * (y_max + 1) * (y_max + 1) <= SIEVE_LIMIT)
            ++y_max;

        prime_cap = 4096;
        prime_list = (seq_t *)malloc(prime_cap * sizeof(seq_t));
        if (!prime_list)
            exit(1);

        for (uint64_t y = 0; y <= y_max; ++y)
        {
            uint64_t yy3 = 3 * y * y;
            for (uint64_t x = 0; ; ++x)
            {
                /* val = 2*x^2 - x*y + 3*y^2 */
                uint64_t val = 2 * x * x - x * y + yy3;
                if (val > SIEVE_LIMIT)
                    break;
                if (val >= 2 && is_prime[val])
                {
                    if (prime_cnt == prime_cap)
                    {
                        prime_cap <<= 1;
                        seq_t *tmp = (seq_t *)realloc(prime_list,
                                                      prime_cap * sizeof(seq_t));
                        if (!tmp)
                            exit(1);
                        prime_list = tmp;
                    }
                    prime_list[prime_cnt++] = (seq_t)val;
                }
            }
        }

        /* 3. Sort the collected primes */
        qsort(prime_list, prime_cnt, sizeof(seq_t), cmp_seq);

        free(is_prime);
        initialized = true;
    }

    /* 4. Return the requested element (1‑based index) */
    if ((size_t)(n - 1) >= prime_cnt)
        return (seq_t)0;            /* undefined for indices beyond precomputed list */
    return prime_list[(size_t)(n - 1)];
}

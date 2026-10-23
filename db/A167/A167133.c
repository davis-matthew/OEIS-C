/*
 * A167133  Primes of the form (number of prime factors of k-th composite)
 *          plus (number of divisors of k-th composite).
 * https://oeis.org/A167133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime equal to (number of prime factors with multiplicity
 *            of the n-th composite) + * (number of divisors of the n-th
 *            composite)
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, divisor_count_u64, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"        /* is_prime(idx_t)         */
#include "../utils/is_prime_big.h"    /* is_prime_big(seq_t)     */
#include "../utils/count_prime_factors.h"  /* count_prime_factors(seq_t) */
#include "../utils/divisor_count_u64.h"    /* divisor_count_u64(uint64_t) */

seq_t a(idx_t n)
{
    /* No sequence value for n = 0 (the sequence starts with offset 1). */
    if (n == 0)
        return 0;

    /* Static state to allow calls in arbitrary order. */
    static seq_t *prime_values = NULL;  /* array of found primes */
    static size_t prime_cap = 0;        /* capacity of prime_values      */
    static size_t prime_count = 0;      /* number of primes stored       */
    static uint64_t next_comp = 4;      /* next integer to test for being a composite */
    /* The following static variables are not strictly necessary, but they keep
       the code clearer. */
    static bool initialized = false;

    if (!initialized)
    {
        prime_cap = 128;
        prime_values = (seq_t *)malloc(prime_cap * sizeof(seq_t));
        if (!prime_values)
            return 0; /* allocation failure – unlikely in test environment */
        initialized = true;
    }

    /* Generate composites and collect qualifying primes until we have at
       least n primes. */
    while (prime_count < n)
    {
        /* Find next composite number. */
        while (true)
        {
            if (next_comp <= 3) { /* 4 is the first composite */
                next_comp = 4;
                break;
            }
            if (!is_prime((idx_t)next_comp))
                break;
            ++next_comp;
        }

        /* Compute omega (number of prime factors with multiplicity) and
           divisor count d(n). */
        seq_t omega = count_prime_factors((seq_t)next_comp);
        uint64_t d = divisor_count_u64(next_comp);
        seq_t sum = omega + (seq_t)d;  /* add d to omega; sum fits in seq_t */

        /* Check if the sum is prime; if so, store it. */
        if (is_prime_big(sum))
        {
            if (prime_count == prime_cap)
            {
                prime_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(prime_values, prime_cap * sizeof(seq_t));
                if (!tmp)
                    return 0; /* allocation failure */
                prime_values = tmp;
            }
            prime_values[prime_count++] = sum;
        }

        /* Advance to the next integer for the next composite search. */
        ++next_comp;
    }

    /* We now have at least n primes stored; return the n-th (1‑based). */
    return prime_values[n - 1];
}

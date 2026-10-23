/*
 * A181709  Indices of primes in A007310.
 * https://oeis.org/A181709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(prime(n+2)/3)+1
 * Range:     u8 n <= 52, u16 n <= 7274, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static storage for the generated primes */
static uint32_t *primes = NULL;
static size_t primes_count = 0;
static size_t primes_capacity = 0;

/* Ensure that we have at least `need` primes stored (1‑based index). */
static void extend_primes(idx_t need)
{
    if (primes_count >= (size_t)need) return;

    /* Initialise the prime table if this is the first call. */
    if (!primes) {
        primes_capacity = 128;
        primes = (uint32_t *)malloc(primes_capacity * sizeof(uint32_t));
        if (!primes) exit(1);
        primes[0] = 2;
        primes[1] = 3;
        primes_count = 2;
    }

    /* Generate primes by trial division. The outer loop runs from the
       last prime + 1 upwards, skipping even numbers. */
    uint32_t num = primes[primes_count - 1] + 1;
    if ((num & 1) == 0) ++num;          /* start with next odd number */

    while (primes_count < (size_t)need) {
        bool is_prime = true;
        /* Check divisibility by known primes not exceeding sqrt(num). */
        for (size_t j = 0; j < primes_count; ++j) {
            uint64_t p = primes[j];
            if (p * p > num) break;
            if ((num % p) == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_count >= primes_capacity) {
                primes_capacity *= 2;
                primes = (uint32_t *)realloc(primes,
                                             primes_capacity * sizeof(uint32_t));
                if (!primes) exit(1);
            }
            primes[primes_count++] = num;
        }
        num += 2; /* skip even numbers */
    }
}

/* Return the n-th term of sequence A181709. */
seq_t a(idx_t n)
{
    /* We need the (n+2)-th prime. */
    idx_t prime_idx = n + 2;
    extend_primes(prime_idx);
    uint64_t p = (uint64_t)primes[prime_idx - 1];   /* array is 0‑based */
    /* Compute floor(p/3) + 1, cast to the correct return type. */
    return ((seq_t)p / 3) + 1;
}

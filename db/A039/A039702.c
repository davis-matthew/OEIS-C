/*
 * A039702  a(n) = n-th prime modulo 4.
 * https://oeis.org/A039702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime modulo 4 (computed by incremental sieve)
 * Range:     u8 n <= 5910, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* Internal storage for primes */
static seq_t *primes = NULL;           /* dynamic array of primes (uint64_t) */
static size_t primes_alloc = 0;        /* allocated size */
static size_t primes_used  = 0;        /* number of primes stored so far */
static uint64_t next_candidate = 3;    /* next odd number to test for primality */

/* Ensure that the first n primes are stored in the array */
static void ensure_primes(idx_t n)
{
    /* Add the first prime explicitly if the list is empty */
    if (primes_used == 0) {
        primes_used = 1;
        if (primes_alloc == 0) {
            primes_alloc = 16;
            primes = (seq_t *)malloc(primes_alloc * sizeof(seq_t));
        }
        primes[0] = 2;
    }

    while (primes_used < n) {
        /* Resize the array if necessary */
        if (primes_used >= primes_alloc) {
            size_t new_alloc = (primes_alloc == 0) ? 16 : primes_alloc * 2;
            seq_t *tmp = (seq_t *)realloc(primes, new_alloc * sizeof(seq_t));
            if (!tmp) abort();  /* abort on allocation failure */
            primes = tmp;
            primes_alloc = new_alloc;
        }

        uint64_t c = next_candidate;
        /* Primality test by trial division using known primes */
        uint64_t limit = (uint64_t) sqrt((double)c);
        bool is_prime = true;
        for (size_t i = 0; i < primes_used && primes[i] <= limit; ++i) {
            if (c % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            primes[primes_used++] = c;
        }
        next_candidate += 2;  /* skip even numbers */
    }
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* undefined for n < 1, but return 0 for safety */
    }

    ensure_primes(n);
    uint64_t prime = (uint64_t)primes[n - 1];
    return (seq_t)(prime % 4);
}

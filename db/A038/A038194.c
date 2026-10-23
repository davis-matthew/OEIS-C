/*
 * A038194  Iterated sum-of-digits of n-th prime; or digital root of n-th
 *          prime; or n-th prime modulo 9.
 * https://oeis.org/A038194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital root of the n-th prime = A010888(A000040(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The digital root of a number equals its value modulo 9, except that
   a multiple of 9 maps to 9.  Since all primes except 3 are not
   multiples of 3, the only prime that can be a multiple of 9 is 3
   itself, whose digital root is 3.  Therefore for all primes p,
   a(n) = p % 9 (with p = prime(n)). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h> /* for sqrt */

static uint64_t *primes = NULL;      /* dynamic array of primes */
static size_t primes_size = 0;       /* number of primes stored */
static size_t primes_cap   = 0;      /* capacity of the array */

/* Ensure that the first n primes are stored in primes[0..n-1] */
static void ensure_primes(idx_t n)
{
    if ((size_t)n <= primes_size) {
        return;
    }

    /* Allocate or grow the primes array */
    if (primes_cap == 0) {
        primes_cap = 32;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }
    while (primes_cap < (size_t)n) {
        primes_cap <<= 1;
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    }

    /* Generate primes starting from 2 */
    uint64_t candidate = (primes_size == 0) ? 2 : primes[primes_size - 1] + 1;
    if (candidate <= 2) {
        /* Include 2 as the first prime */
        primes[primes_size++] = 2;
        candidate = 3;
    }

    /* Subsequent candidates are odd numbers only */
    if ((candidate & 1) == 0) {
        candidate++;
    }

    while (primes_size < (size_t)n) {
        bool is_prime = true;
        uint64_t limit = (uint64_t)floor(sqrt((double)candidate));
        for (size_t i = 0; i < primes_size && primes[i] <= limit; ++i) {
            if (candidate % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            primes[primes_size++] = candidate;
        }
        candidate += 2; /* skip even numbers */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n==0; return 0 as a safe convention */
        return (seq_t)0;
    }
    ensure_primes(n);
    /* The digital root of any prime p is p % 9, except that 0 would
       represent 9, but no prime is a multiple of 9. */
    seq_t res = (seq_t)(primes[n - 1] % 9);
    return res;
}

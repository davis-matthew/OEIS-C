/*
 * A091801  Largest n-digit multiple of the n-th prime.
 * https://oeis.org/A091801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = largest n‑digit multiple of the n‑th prime (offset 1).  */
/* The function builds the n‑th prime by trial division, then
   performs long division of the decimal number consisting of n
   digits '9' by that prime, and finally multiplies the quotient
   by the prime to obtain the desired multiple.  */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Global storage for primes, rebuilt only when more primes are needed. */
static idx_t *prime_list = NULL;   /* array of primes */
static size_t prime_count = 0;     /* number of stored primes */
static size_t prime_capacity = 0;  /* current allocation size */

/* ------------------------------------------------------------------ */
/* Ensure that prime_list contains at least n primes.  */
static void
ensure_primes(idx_t n)
{
    while (prime_count < n) {
        idx_t candidate = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;

        /* Find the next prime */
        while (true) {
            idx_t i;
            double d = sqrt((double)candidate);
            idx_t limit = (idx_t)d;
            bool is_prime = true;
            for (i = 0; i < prime_count && prime_list[i] <= limit; ++i) {
                if (candidate % prime_list[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                /* Append candidate to the list */
                if (prime_count == prime_capacity) {
                    size_t new_capacity = (prime_capacity == 0) ? 32 : prime_capacity * 2;
                    idx_t *tmp = (idx_t *)realloc(prime_list, new_capacity * sizeof(idx_t));
                    if (!tmp) {
                        /* Memory allocation failed; terminate */
                        exit(1);
                    }
                    prime_list = tmp;
                    prime_capacity = new_capacity;
                }
                prime_list[prime_count++] = candidate;
                break;
            }
            ++candidate;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1 by the sequence definition */
    ensure_primes(n);
    idx_t prime = prime_list[n - 1];        /* nth prime (1‑based) */

    /* Long division of 99…9 (n digits) by 'prime' */
    idx_t rem = 0;
    seq_t quotient = (seq_t)0;

    for (idx_t i = 0; i < n; ++i) {
        rem = rem * 10 + 9;
        idx_t digit = rem / prime;    /* quotient digit */
        rem   = rem % prime;           /* remainder */
        quotient = quotient * (seq_t)10 + (seq_t)digit;
    }

    /* The required multiple is quotient * prime */
    seq_t result = quotient * (seq_t)prime;
    return result;
}

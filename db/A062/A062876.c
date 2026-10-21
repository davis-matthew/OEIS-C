/*
 * A062876  Numbers of lattice points corresponding to incrementally largest
 *          circle radii in A062875.
 * https://oeis.org/A062876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4 * prime(n) for n >= 2, a(1) = 4 (the first term)
 * Range:     u8 n <= 18, u16 n <= 1900, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for the successive primes */
static idx_t *prime_list   = NULL;
static size_t prime_count  = 0;
static size_t prime_capacity = 0;

/* Initialise or grow the prime array to contain at least 'need' primes.  */
static void ensure_primes(idx_t need)
{
    /* initialise the table with the prime 2 */
    if (prime_count == 0) {
        prime_capacity = 16;
        prime_list = (idx_t *)malloc(prime_capacity * sizeof(idx_t));
        prime_list[0] = 2;
        prime_count  = 1;
    }

    /* generate further primes until we have 'need' of them */
    while (prime_count < need) {
        /* start with the next odd candidate after the last prime */
        idx_t candidate = prime_list[prime_count - 1] + 1;
        if ((candidate & 1) == 0) {
            candidate++;               /* make sure we start at an odd number */
        }

        /* test primality by trial division using the primes we already know */
        while (1) {
            int is_prime = 1;
            for (size_t i = 0; i < prime_count; ++i) {
                idx_t p = prime_list[i];
                /* stop when p^2 > candidate */
                if ((p * p) > candidate) {
                    break;
                }
                if (candidate % p == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) {
                break;                  /* candidate is prime */
            }
            candidate += 2;              /* try the next odd number */
        }

        /* expand the array if necessary */
        if (prime_count == prime_capacity) {
            prime_capacity *= 2;
            prime_list = (idx_t *)realloc(prime_list, prime_capacity * sizeof(idx_t));
        }

        prime_list[prime_count++] = candidate;   /* store the new prime */
    }
}

/* Return the n‑th term of sequence A062876 */
seq_t a(idx_t n)
{
    if (n == 1) {
        return 4;                /* special first term */
    }

    ensure_primes(n);                /* make sure we have the nth prime */

    idx_t p = prime_list[n - 1];     /* nth prime (1‑based) */
    return (seq_t)4 * p;             /* a(n) = 4 * prime(n) for n >= 2 */
}

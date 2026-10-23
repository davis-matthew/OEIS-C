/*
 * A133063  a(n) = 5*p^5 + 3*p^3 - 2*p^2, where p = prime(n).
 * https://oeis.org/A133063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*p^5 + 3*p^3 - 2*p^2, where p = prime(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 18, u64 n <= 688, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function builds a list of primes incrementally using trial division.
   It keeps a static array of found primes so repeated calls to a(n) are
   cached.  The function is safe for any call order and works for the
   arbitrary‑precision (C++ big integer) build because the type seq_t can
   be constructed from a uint64_t.  The compiler fatal cast for realloc
   is not needed in C, but is supplied for C++.  All intermediate values
   are computed with seq_t arithmetic to avoid overflow. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)                      /* offset starts at 1 */
        return (seq_t)0;

    /* static cache of primes so repeated calls are fast */
    static uint64_t *prime_list = NULL;   /* primes found so far */
    static size_t prime_count = 0;        /* number of primes stored */
    static size_t prime_capacity = 0;     /* current allocator capacity */
    static uint64_t candidate = 2;        /* next integer to test */

    /* Generate primes until we have at least n of them. */
    while (prime_count < (size_t)n) {
        uint64_t p = candidate;
        bool is_prime = true;

        /* Trial division using primes already found. */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t q = prime_list[i];
            if (q * q > p)          /* q^2 surpasses candidate -> stop */
                break;
            if (p % q == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Grow the array if necessary. */
            if (prime_count == prime_capacity) {
                size_t new_cap = (prime_capacity == 0) ? 16 : prime_capacity * 2;
                void *tmp = realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp) {         /* allocation failure – abort */
                    exit(EXIT_FAILURE);
                }
                prime_list = (uint64_t *)tmp;   /* cast for C++ */
                prime_capacity = new_cap;
            }
            prime_list[prime_count++] = p;
        }

        /* Advance to next candidate number. */
        if (candidate == 2) {
            candidate = 3;
        } else {
            candidate += 2;          /* skip even numbers */
        }
    }

    /* Retrieve the n‑th prime (1‑based). */
    seq_t p = (seq_t)prime_list[n - 1];

    /* Compute p^2, p^3, p^5 in seq_t arithmetic. */
    seq_t p2 = p * p;         /* p^2 */
    seq_t p3 = p2 * p;        /* p^3 */
    seq_t p5 = p3 * p2;       /* p^5 */

    /* Evaluate the polynomial. */
    seq_t result = 5 * p5 + 3 * p3 - 2 * p2;
    return result;
}

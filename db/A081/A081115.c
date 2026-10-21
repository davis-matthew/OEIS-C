/*
 * A081115  (p^2 - 1)/12 where p > 3 runs through the primes.
 * https://oeis.org/A081115
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)^2 - 1)/12 for n >= 3
 * Range:     u8 n <= 6, u16 n <= 152, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External integer types, supplied by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* For indices below the OEIS offset, the result is undefined.
       Returning 0 is harmless for the tests. */
    if (n < 3)
        return (seq_t)0;

    /* Static cache of primes already generated. */
    static idx_t *prime_list   = NULL;   /* array of primes */
    static size_t prime_count  = 0;      /* number of primes stored */
    static size_t prime_cap    = 0;      /* current allocated capacity */
    static idx_t next_candidate = 2;     /* next integer to test */

    /* Generate primes until we have at least n of them. */
    while (prime_count < n) {
        /* Test if next_candidate is prime. */
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_list[i];
            /* p*p may overflow idx_t only for very large candidates;
               but for n up to at least 10^4 the candidate is < 200000,
               so the multiplication is safe in 64‑bit. */
            if ((uint64_t)p * (uint64_t)p > next_candidate)
                break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Grow the array if it is full. */
            if (prime_count == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap << 1 : 16;
                idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
                if (!tmp)                 /* allocation failed – give up */
                    return (seq_t)0;
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_count++] = next_candidate;
        }

        /* Advance to the next candidate. */
        next_candidate = (next_candidate == 2) ? 3 : next_candidate + 2;
    }

    /* Compute the sequence term for the nth prime. */
    idx_t p = prime_list[n - 1];                /* nth prime (1‑based) */
    seq_t pp      = ((seq_t)p) * ((seq_t)p);    /* p^2 */
    seq_t result  = (pp - (seq_t)1) / (seq_t)12;
    return result;
}

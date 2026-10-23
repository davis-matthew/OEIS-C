/*
 * A141877  Primes congruent to 12 mod 19.
 * https://oeis.org/A141877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 12 (mod 19)
 * Range:     u8 n <= 2, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* The build supplies the types seq_t and idx_t; we assume idx_t is an
 * unsigned integer type (e.g., uint64_t).  No other external symbols are
 * required from the utilities listed in the problem statement. */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* Rough upper bound for the n-th prime in the residue class 12 mod 19.
     * The asymptotic estimate is a(n) ~ 18 * n * log n.  We use a safety
     * factor of 20 and ensure a minimum limit of 100. */
    double dn = (double)n;
    double est = dn * (log(dn) + log(log(dn))); /* 18*n*log n approx */
    if (est <= 0.0) est = 10.0;
    idx_t limit = (idx_t)(est * 20.0);
    if (limit < 100) limit = 100;

    while (1) {
        /* Allocate a flag array for the sieve (true = prime). */
        bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
        if (!is_prime) return (seq_t)0; /* allocation failed */

        /* Initialize sieve: all numbers 2..limit are presumed prime. */
        for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;

        /* Sieve of Eratosthenes */
        for (idx_t p = 2; p * p <= limit; ++p) {
            if (is_prime[p]) {
                for (idx_t multiple = p * p; multiple <= limit; multiple += p)
                    is_prime[multiple] = false;
            }
        }

        /* Count primes congruent to 12 modulo 19. */
        idx_t count = 0;
        seq_t result = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (is_prime[i] && (i % 19u == 12u)) {
                ++count;
                if (count == n) {
                    result = (seq_t)i;
                    break;
                }
            }
        }

        free(is_prime);

        if (result != 0) return result;          /* found */
        /* Not enough primes found – double the limit and try again. */
        limit *= 2;
    }
}

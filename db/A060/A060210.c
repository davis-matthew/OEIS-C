/*
 * A060210  Largest prime factor of 1+smaller term of twin primes.
 * https://oeis.org/A060210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of (p_n + 1), where p_n is the
 *            smaller member of the nth twin prime pair
 * Range:     u8 n <= 53, u16 n <= 3787, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Helpers from the utils directory */
#include "../utils/is_prime.h"          /* bool is_prime(idx_t x); */
#include "../utils/largest_prime_factor.h" /* uint64_t largest_prime_factor(uint64_t n); */

/*
 * Return the nth term of OEIS sequence A060210.
 * Handles any order of calls; no caching used – the cost is
 * negligible for n up to about 10 000.
 */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1 */

    idx_t count = 0;      /* number of twin primes found  */
    idx_t p = 3;          /* current candidate for the smaller prime */

    while (true) {
        /* Check if p and p+2 are both prime.  */
        if (is_prime(p) && is_prime(p + 2)) {
            ++count;
            if (count == n) {
                /* Compute largest prime factor of p+1.  */
                uint64_t lpf = largest_prime_factor((uint64_t)p + 1ULL);
                return (seq_t)lpf;
            }
        }

        /* Advance to next odd number (k must remain odd for prime test). */
        p += 2;
    }
}

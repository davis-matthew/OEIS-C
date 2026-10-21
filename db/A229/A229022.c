/*
 * A229022  a(n) = sopf(n) + n/rad(n).
 * https://oeis.org/A229022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sopf(n) + n / rad(n) sopf(n) = sum of distinct prime
 *            factors of n rad(n) = product of distinct prime factors of n
 *            a(1) = 1 since sopf(1)=0 and rad(1)=1
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based, but calculations are 64‑bit (idx_t) */
    if (n == 0) {
        /* undefined for n=0, return 0 to avoid UB */
        return (seq_t)0;
    }

    seq_t sopf = (seq_t)0;          /* sum of distinct primes */
    seq_t rad  = (seq_t)1;          /* product of distinct primes */

    idx_t temp = n;                 /* working copy for factorisation */

    /* Trial division up to sqrt(temp).  For n <= 10000 this loop is trivial. */
    for (idx_t p = 2; (idx_t)p * p <= temp; ++p) {
        if (temp % p == 0) {
            sopf += (seq_t)p;      /* p is a distinct prime factor */
            rad  *= (seq_t)p;

            /* Remove all copies of p from temp */
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }

    /* If a prime factor larger than sqrt remains */
    if (temp > 1) {
        sopf += (seq_t)temp;
        rad  *= (seq_t)temp;
    }

    /* n / rad(n) is an integer since rad(n) divides n */
    seq_t quotient = (seq_t)n / rad;
    return sopf + quotient;
}

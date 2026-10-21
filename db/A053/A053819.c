/*
 * A053819  a(n) = Sum_{k=1..n, gcd(n,k) = 1} k^3.
 * https://oeis.org/A053819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n)*(n^3 + (-1)^omega(n)*rad(n)*n)/4 for n>1, a(1) =
 *            1. Here phi is Euler's totient, rad is the product of distinct
 *            prime factors, and omega is the number of distinct prime factors
 *            (given via count_distinct_primes)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 359, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, phi, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/rad.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t sn    = (seq_t)n;
    seq_t n3    = sn * sn * sn;          /* n^3  */
    seq_t rad_n = rad(n);                /* rad(n) */
    seq_t phi_n = phi(n);                /* phi(n) */
    int d       = count_distinct_primes((uint64_t)n); /* omega(n) */
    seq_t mul   = rad_n * sn;            /* rad(n) * n */

    seq_t term = n3;
    if (d & 1) {                         /* odd number of primes: subtract */
        term -= mul;
    } else {                             /* even number: add */
        term += mul;
    }

    term *= phi_n;                       /* multiply by phi(n) */
    term /= (seq_t)4;                    /* divide by 4, integer result */

    return term;
}

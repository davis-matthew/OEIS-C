/*
 * A112323  a(n) = numerator of sum of reciprocals of the terms of the
 *          continued fraction for prime(n+1)/prime(n).
 * https://oeis.org/A112323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the sum of reciprocals of the terms of the
 *            continued fraction for prime(n+1)/prime(n)
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_any, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_any.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /*  n >= 1 (the OEIS offset).  For n < 1 the function is not
        used; return 0 as a harmless default. */
    if (n < 1) {
        return (seq_t)0;
    }

    /* Continuous fraction of q/p where p = prime(n) and q = prime(n+1). */
    const seq_t p = get_prime((uint64_t)n);           /* denominator   */
    const seq_t q = get_prime((uint64_t)(n + 1U));    /* numerator     */

    /* Sum of reciprocals, kept in reduced form. */
    seq_t sum_num = 0;      /* numerator of the current sum   */
    seq_t sum_den = 1;      /* denominator of the current sum */

    /* Euclid variables for the continued fraction. */
    seq_t a_i;      /* current quotient        */
    seq_t r;        /* remainder in Euclid step */
    seq_t numer = q;
    seq_t denom = p;

    while (denom != 0) {
        /* Next continued‑fraction term: floor(numer/denom). */
        a_i = numer / denom;

        /* Add 1/a_i to the sum.   (sum_num/sum_den) + 1/a_i  */
        seq_t new_num = sum_num * a_i + sum_den;
        seq_t new_den = sum_den * a_i;

        /* Reduce the fraction. */
        seq_t g = gcd_any(new_num, new_den);
        sum_num = new_num / g;
        sum_den = new_den / g;

        /* Prepare next Euclid step. */
        r = numer % denom;
        numer = denom;
        denom = r;
    }

    /* Return the numerator of the final sum. */
    return sum_num;
}

/*
 * A088529  Numerator of Bigomega(n)/Omega(n).
 * https://oeis.org/A088529
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of bigomega(n) / omega(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, count_distinct_primes, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/gcd.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* a(n) is defined only for n >= 2. For other arguments we return 0. */
    if (n < 2)
        return 0;

    seq_t B = bigomega(n);                /* total prime factors with multiplicity */
    int   O_int = count_distinct_primes(n); /* number of distinct prime factors   */
    seq_t O = (seq_t)O_int;                /* convert to seq_t for arithmetic    */

    seq_t g = gcd(B, O);                   /* greatest common divisor           */
    return B / g;                          /* numerator of the reduced fraction */
}

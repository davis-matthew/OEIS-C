/*
 * A088530  Denominator of bigomega(n)/omega(n).
 * https://oeis.org/A088530
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) = omega(n) / gcd( bigomega(n), omega(n) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, count_distinct_primes, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the denominator of bigomega(n)/omega(n) for n >= 2 */
#include <stdint.h>
#include "../utils/gcd.h"     /* provides seq_t gcd(seq_t, seq_t) */
#include "../utils/bigomega.h" /* provides seq_t bigomega(idx_t) */
#include "../utils/count_distinct_primes.h" /* provides int count_distinct_primes(uint64_t) */

seq_t a(idx_t n)
{
    /* Big omega (total number of prime factors with multiplicity) */
    seq_t b = bigomega(n);

    /* Omega (number of distinct prime factors) */
    seq_t o = (seq_t)count_distinct_primes((uint64_t)n);

    /* Greatest common divisor of b and o */
    seq_t g = gcd(b, o);

    /* Denominator of the reduced fraction b / o */
    return o / g;
}

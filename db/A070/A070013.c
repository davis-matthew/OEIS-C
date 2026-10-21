/*
 * A070013  Number of prime factors of n divided by the number of n's distinct
 *          prime factors (rounded).
 * https://oeis.org/A070013
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(Ω(n)/ω(n)) for n ≥ 2 Ω(n) = total number of prime
 *            factors of n (with multiplicity) ω(n) = number of distinct prime
 *            factors of n Uses the helper functions bigomega() and
 *            count_distinct_prime_factors()
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega, count_distinct_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/count_distinct_prime_factors.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* below offset – undefined, return 0 */

    seq_t big   = bigomega(n);                          /* Ω(n) */
    int  omega = count_distinct_prime_factors((uint64_t)n); /* ω(n) */

    /* rounded average: floor((Ω(n) + ω(n)/2) / ω(n)) */
    seq_t numerator = big + (seq_t)(omega / 2);
    return numerator / (seq_t)omega;
}

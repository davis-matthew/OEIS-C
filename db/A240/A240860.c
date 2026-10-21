/*
 * A240860  a(n) = Sum_{i=1..n} (-1)^{i+1} prime(i)^2, where prime(k) denotes
 *          the k-th prime: alternating sum of the squares of the first n
 *          primes.
 * https://oeis.org/A240860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{i=1..n} (-1)^{i+1} * prime(i)^2
 * Range:     i8 n <= 6, i16 n <= 50, i32 n <= 6486, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* External linkage, exact signature required */
seq_t a(idx_t n)
{
    /*  n ≥ 1 is guaranteed by the problem statement. */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t p   = (seq_t)get_prime(i); /* i‑th prime */
        seq_t p2  = p * p;               /* square of the prime    */
        if (i & 1U)                      /* odd i → add           */
            sum += p2;
        else                              /* even i → subtract      */
            sum -= p2;
    }
    return sum;
}

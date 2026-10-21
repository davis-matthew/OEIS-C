/*
 * A071862  Number of elements in the continued fraction for Sum_{d|n} 1/d.
 * https://oeis.org/A071862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1749, u16 n <= 8948, u32 n <= 8948, u64 n <= 8948,
 *            u128 n <= 8948, big any n
 * Helpers:   continued_fraction_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of elements in the simple continued fraction of  σ(n)/n,
 * where σ(n) is the sum of divisors of n.
 *
 * The implementation computes σ(n) by factoring n, builds the rational
 * σ(n)/n, and counts the terms of its simple continued fraction using
 * the Euclidean algorithm.
 */
#include <stdbool.h>
#include "../utils/continued_fraction_length.h"

seq_t a(idx_t n)
{
    /* Compute σ(n) */
    seq_t sigma = 1;           /* product of sigma(p^e) over prime factors */
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            /* Count exponent e of prime p */
            idx_t e = 0;
            seq_t pow = 1;
            seq_t term = 1;   /* sigma(p^e) = 1 + p + ... + p^e */
            while (m % p == 0) {
                m /= p;
                ++e;
                pow *= (seq_t)p;
                term += pow;
            }
            sigma *= term;
        }
    }
    if (m > 1) {              /* remaining prime factor */
        seq_t term = 1 + (seq_t)m;
        sigma *= term;
    }

    /* Count terms of continued fraction of σ(n)/n */
    seq_t num = sigma;
    seq_t den = (seq_t)n;   /* denominator n */
    return continued_fraction_length(num, den);
}

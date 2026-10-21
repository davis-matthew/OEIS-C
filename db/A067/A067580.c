/*
 * A067580  a(n) = Product_{i=2..n} A001222(i) * Sum_{i=2..n} 1/A001222(i).
 * https://oeis.org/A067580
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=2..n} Ω(i) * Sum_{i=2..n} 1/Ω(i) where Ω(i) is
 *            the number of prime factors of i counted with multiplicity
 *            (bigOmega). For each i, Ω(i) ≥ 1, so all divisions are exact
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 61, u128 n <= 113,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* below the sequence offset */

    /* Compute the complete product of Ω(i) for i = 2..n. */
    seq_t prod = 1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t omega = bigomega(i);
        prod *= omega;
    }

    /* Compute the sum of prod / Ω(i).  All divisions are exact. */
    seq_t sum = 0;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t omega = bigomega(i);
        sum += prod / omega;
    }

    return sum;
}

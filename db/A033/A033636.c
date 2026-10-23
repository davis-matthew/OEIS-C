/*
 * A033636  a(n) = (n-1)! * Product_{i=1..n-1} ((2*n)!-2*i-1).
 * https://oeis.org/A033636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)! * Product_{i=1..n-1} ((2*n)!-2*i-1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t one = 1;
    // (n-1)!
    seq_t fact_n_minus_1 = one;
    for (idx_t i = 2; i < n; ++i) {
        fact_n_minus_1 *= (seq_t)i;
    }

    // (2*n)!
    seq_t fact_2n = 2;                     // start with 2! = 2
    for (idx_t i = 3; i <= 2 * n; ++i) {
        fact_2n *= (seq_t)i;
    }

    // Product over i = 1 .. n-1 of (fact_2n - (2*i + 1))
    seq_t prod = one;
    for (idx_t i = 1; i < n; ++i) {
        seq_t term = fact_2n - ((seq_t)2 * i + 1);
        prod *= term;
    }

    return fact_n_minus_1 * prod;
}

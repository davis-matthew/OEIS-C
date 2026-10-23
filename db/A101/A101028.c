/*
 * A101028  Numerator of partial sums of a certain series. First member (m =
 *          2) of a family.
 * https://oeis.org/A101028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator ( 6 * Sum_{k=1..n} 1 / ((2*k-1)*(2*k)*(2*k+1))
 *            ) in lowest terms
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   // for standard integer types

/* Euclidean gcd for seq_t (works for native or arbitrary‑precision integer types) */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Sequence a(n): numerator of the partially summed series */
seq_t a(idx_t n)
{
    /* current sum as reduced fraction p/q */
    seq_t p = 0;          /* numerator */
    seq_t q = 1;          /* denominator */

    for (idx_t k = 1; k <= n; ++k) {
        /* Compute the k-th term denominator: (2k-1)(2k)(2k+1) */
        seq_t a_term = (seq_t)(2*k - 1);
        seq_t b_term = (seq_t)(2*k);
        seq_t c_term = (seq_t)(2*k + 1);

        seq_t denom_k = a_term * b_term * c_term;   /* (2k-1)(2k)(2k+1) */

        /* Reduce the new term against the current denominator first */
        seq_t g1 = seq_gcd(q, denom_k);
        seq_t denom_k_reduced = denom_k / g1;
        seq_t q_reduced = q / g1;

        /* Add the new term to the current sum */
        seq_t new_p = p * denom_k_reduced + (seq_t)6 * q_reduced;
        seq_t new_q = q * denom_k_reduced;

        /* Reduce the resulting fraction */
        seq_t g2 = seq_gcd(new_p, new_q);
        p = new_p / g2;
        q = new_q / g2;
    }

    return p;   /* numerator of the reduced fraction */
}

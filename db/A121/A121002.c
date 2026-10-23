/*
 * A121002  Numerators of partial sums of Catalan numbers scaled by powers of
 *          1/5.
 * https://oeis.org/A121002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of sum_{k=0}^n C(k)/5^k in lowest terms
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by the recurrence
 *   N_0 = 1
 *   N_n = 5·N_{n-1} + C(n)  for n ≥ 1,
 * where C(n) is the nth Catalan number.
 * The rational r(n) = N_n / 5^n.
 * The numerator of r(n) in lowest terms is a(n),
 * i.e. a(n) = N_n / 5^{v_5(N_n)} where v_5 is the exponent of 5 in N_n.
 *
 * The implementation computes the unreduced numerator N_n iteratively,
 * then removes all factors of 5 to obtain a(n).
 */

#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Constants represented as seq_t */
    const seq_t one  = seq_from_str("1");
    const seq_t five = seq_from_str("5");

    /* Current unreduced numerator N_n */
    seq_t N = one;

    /* Current Catalan number, starting with C(0)=1 */
    seq_t cat = one;

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute next Catalan number using C_{i} = C_{i-1} * 2*(2*(i-1)+1) / (i+1) */
        seq_t k_minus1 = (seq_t)(i - 1);
        seq_t mult = (seq_t)2 * ((seq_t)2 * k_minus1 + (seq_t)1);   /* 2*(2*(i-1)+1) */
        cat = cat * mult;
        cat = cat / ((seq_t)(i + 1));   /* exact division */

        /* Update unreduced numerator: N_i = 5*N_{i-1} + C(i) */
        N = N * five + cat;
    }

    /* Remove all factors of 5 to obtain the numerator in lowest terms */
    while (N % five == 0) {
        N = N / five;
    }

    return N;
}

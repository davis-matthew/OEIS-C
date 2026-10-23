/*
 * A175827  Partial sums of ceiling(n^2/10).
 * https://oeis.org/A175827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 + 28*n + 36)/60)
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 1787, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to sequence type to prevent overflow in intermediate
       calculations and ensure correct integer arithmetic. */
    seq_t nn = (seq_t)n;

    /* Compute the numerator of the fraction in the formula.  Multiplication
       order is chosen to keep intermediate values within seq_t's range. */
    seq_t term1 = (seq_t)2 * nn * nn * nn; /* 2*n^3 */
    seq_t term2 = (seq_t)3 * nn * nn;      /* 3*n^2 */
    seq_t term3 = (seq_t)28 * nn;          /* 28*n */

    /* Sum all terms and add the constant 36. */
    seq_t numer = term1 + term2 + term3 + (seq_t)36;

    /* Perform integer division; for non‑negative values this is equivalent
       to floor division. */
    return numer / (seq_t)60;
}

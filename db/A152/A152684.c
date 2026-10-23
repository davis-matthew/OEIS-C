/*
 * A152684  a(n) is the number of top-down sequences (F_1, F_2, ..., F_n)
 *          whereas each F_i is a labeled forest on n nodes, containing i
 *          directed rooted trees. F_(i+1) is proper subset of F_i.
 * https://oeis.org/A152684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-2)*n!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle cases where the exponent would be negative.  For n = 1,
       1^(–1) is treated as 1; for n = 2, 2^0 = 1. */
    seq_t pow_term = 1;
    if (n >= 3) {
        seq_t base = (seq_t)n;
        for (idx_t k = 0; k < n - 2; ++k) {
            pow_term *= base;
        }
    }

    /* Compute n! iteratively.  Use up to n; for n = 0 or 1, the loop
       does not execute and factorial remains 1. */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    return fact * pow_term;
}

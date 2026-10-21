/*
 * A062199  Second (unsigned) column sequence of triangle A062140 (generalized
 *          a=4 Laguerre).
 * https://oeis.org/A062199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)! * C(n+5,5)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t binom = 1;
    /* Compute C(n+5,5) as product of (n+i)/i for i=1..5 */
    for (idx_t i = 1; i <= 5; i++) {
        seq_t num = (seq_t)n + (seq_t)i;
        binom = binom * num / (seq_t)i;
    }

    seq_t fact = 1;
    for (idx_t k = 2; k <= n + 1; k++) {
        fact = fact * (seq_t)k;
    }

    return binom * fact;
}

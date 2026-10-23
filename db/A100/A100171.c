/*
 * A100171  Structured triakis octahedral numbers (vertex structure 4).
 * https://oeis.org/A100171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * ((7*n*n - 9*n + 4) / 2) (integer division, numerator
 *            always even)
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 1071, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n;            /* n */
    seq_t n2 = n1 * n1;             /* n^2; safe for all n where a(n) fits in seq_t */
    seq_t T  = (seq_t)7 * n2 - (seq_t)9 * n1 + (seq_t)4; /* 7*n^2 - 9*n + 4 */
    seq_t t2 = T >> 1;              /* divide by 2; T is always even */
    return n1 * t2;
}

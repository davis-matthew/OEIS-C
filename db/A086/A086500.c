/*
 * A086500  Group the natural numbers such that the n-th group sum is
 *          divisible by the n-th triangular number: (1), (2, 3, 4), (5, 6,
 *          7), (8, 9, 10, 11, 12), (13, 14, 15, 16, 17), (18, 19, 20, 21, 22,
 *          23, 24), ... Sequence contains the group sum.
 * https://oeis.org/A086500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n+1+(-1)^n)/4
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of the n-th group in OEIS A086500 */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;                 /* convert index to seq_t early */
    seq_t tri   = n_seq * (n_seq + 1) / 2;  /* triangular number T(n) */

    /* For odd n: a = T(n) * n
       For even n: a = T(n) * (n+1)            */
    return (n & 1) ? (tri * n_seq) : (tri * (n_seq + 1));
}

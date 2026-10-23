/*
 * A177342  a(n) = (4*n^3-3*n^2+5*n-3)/3.
 * https://oeis.org/A177342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 - 3*n^2 + 5*n - 3) / 3
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1057, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset 1: a(1) is the first term. */

seq_t a(idx_t n)
{
    /* Protect against n == 0; the sequence is defined for n >= 1. */
    if (n == 0)
        return 0;

    seq_t N = (seq_t)n;

    seq_t numerator = ((seq_t)4 * N * N * N)               /* 4*n^3 */
                     - ((seq_t)3 * N * N)                 /* -3*n^2 */
                     + ((seq_t)5 * N)                    /* +5*n */
                     - (seq_t)3;                         /* -3 */

    return numerator / (seq_t)3;
}

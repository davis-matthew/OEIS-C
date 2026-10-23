/*
 * A168582  a(n) = (4*n^3 - 6*n^2 + 8*n + 9 + 3*(-1)^n)/12.
 * https://oeis.org/A168582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 - 6*n^2 + 8*n + 9 + 3*(-1)^n)/12
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1701, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t to avoid signed/unsigned mismatches */
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n3  = n2 * nn;
    /* term = 12 when n even, 6 when n odd  (since 9 + 3*(-1)^n) */
    seq_t term = (n & (seq_t)1) ? (seq_t)6 : (seq_t)12;

    /* compute numerator safely: (4*n3 + 8*nn + term) - 6*n2 */
    seq_t sum = 4 * n3 + 8 * nn + term;
    seq_t num = sum - 6 * n2;

    return num / (seq_t)12;
}

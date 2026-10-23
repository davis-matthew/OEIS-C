/*
 * A167587  The fifth row of the ED4 array A167584: 80*n^4 + 952*n^2 - 768*n +
 *          525.
 * https://oeis.org/A167587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 80*n^4 + 952*n^2 - 768*n + 525
 * Range:     u8 none, u16 n <= 4, u32 n <= 85, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid implicit narrowing. */
    seq_t N  = (seq_t)n;
    /* Compute n^2 and n^4 using unsigned arithmetic. */
    seq_t n2 = N * N;
    seq_t n4 = n2 * n2;
    /* Apply the polynomial. */
    seq_t term1 = 80 * n4;
    seq_t term2 = 952 * n2;
    seq_t term3 = 768 * N;
    return term1 + term2 - term3 + 525;
}

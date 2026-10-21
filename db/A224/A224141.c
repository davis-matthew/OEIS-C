/*
 * A224141  Number of n X 3 0..1 arrays with rows and antidiagonals unimodal
 *          and columns nondecreasing.
 * https://oeis.org/A224141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^3 + 6*n^2 + 13*n) / 3 (valid for n ≥ 1)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                 /* the offset starts at 1; unused case */
        return 0;

    seq_t ns = (seq_t)n;        /* promote n to seq_t */

    /* compute the numerator 2*n^3 + 6*n^2 + 13*n */
    seq_t term1 = (seq_t)2 * ns * ns * ns;   /* 2*n^3   */
    seq_t term2 = (seq_t)6 * ns * ns;         /* 6*n^2   */
    seq_t term3 = (seq_t)13 * ns;             /* 13*n    */

    return (term1 + term2 + term3) / (seq_t)3; /* exact division by 3 */
}

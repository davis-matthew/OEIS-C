/*
 * A027320  Duplicate of A027319.
 * https://oeis.org/A027320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0)=1, a(1)=3, a(2)=8, a(3)=20; for n ≥ 4: \
 *            a(n)=8·(n+1)·3^(n-3)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 12, i64 n <= 12, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)20;

    /* Compute 3^(n-3) by repeated multiplication with the small constant 3 */
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < n - 3; ++i)
        pow = pow * (seq_t)3;

    /* Compute 8*(n+1)*pow */
    seq_t result = (seq_t)8 * (seq_t)(n + 1);
    result = result * pow;
    return result;
}

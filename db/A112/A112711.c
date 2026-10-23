/*
 * A112711  Partial sums of Catalan numbers A000108 multiplied by powers of
 *          -4.
 * https://oeis.org/A112711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sum_{k=0}^{n} Catalan(k)*(-4)^k
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)1;          /* a(0) */
    seq_t catalan = (seq_t)1;      /* C0 */
    seq_t pow_neg4 = (seq_t)1;     /* (-4)^0 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute C_k = C_{k-1} * 2*(2*(k-1)+1) / (k+1) */
        seq_t num = (seq_t)2 * (seq_t)(2 * (k - 1) + 1);
        catalan = (catalan * num) / ((seq_t)k + 1);

        /* update (-4)^k */
        pow_neg4 = pow_neg4 * ((seq_t)-4);

        sum += catalan * pow_neg4;
    }
    return sum;
}

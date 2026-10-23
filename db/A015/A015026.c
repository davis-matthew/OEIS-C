/*
 * A015026  q-factorial numbers for q=-11.
 * https://oeis.org/A015026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; a(n) = ((-11)^n - 1) * a(n-1) / (-12) for n>0
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* iterate multiplicatively: keep current power of -11 */
    seq_t result = (seq_t)1;
    seq_t power   = (seq_t)(-11);   /* (-11)^1 */
    const seq_t denom = (seq_t)(-12);

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        seq_t term = power - (seq_t)1;      /* ((-11)^k - 1) */
        result = (result * term) / denom;   /* multiply and divide */
        power = power * (seq_t)(-11);       /* update power to (-11)^(k+1) */
    }

    return result;
}

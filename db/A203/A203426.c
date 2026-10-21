/*
 * A203426  Reciprocal of Vandermonde determinant of (1/4,1/6,...,1/(2n+2)).
 * https://oeis.org/A203426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = product_{k=1..n} k * (-2(k+1))^(k-1)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t exp = k - 1;
        /* compute |(-2(k+1))|^(k-1) as (2(k+1))^exp */
        seq_t base_abs = (seq_t)(2 * (k + 1));

        /* fast exponentiation on seq_t */
        seq_t pow_abs = 1;
        idx_t e = exp;
        while (e > 0)
        {
            if (e & 1U)
                pow_abs *= base_abs;
            base_abs *= base_abs;
            e >>= 1;
        }

        /* adjust sign: (-2(k+1))^(k-1) = -(2(k+1))^(k-1) if exp is odd */
        if (exp & 1U)
            pow_abs = -pow_abs;

        /* term = k * (-2(k+1))^(k-1) */
        seq_t term = (seq_t)k * pow_abs;
        result *= term;
    }
    return result;
}

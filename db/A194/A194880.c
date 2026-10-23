/*
 * A194880  The numerators of the inverse Akiyama-Tanigawa algorithm from
 *          A001045(n).
 * https://oeis.org/A194880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, frac, easy
 * Formula:   a(3*n) = -3*n - 1 (a(0)=0) a(3*n+1) = -3*n - 2 (a(1)=-1)
 *            a(3*n+2) = -n - 1 Compute a(n) for any index n >= 0
 * Range:     i8 n <= 128, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n % 3)
    {
        case 0:                     /* indices 0,3,6,9,... */
            if (n == 0) return (seq_t)0;
            return (seq_t)(-3 * ((seq_t)(n / 3)) - 1);
        case 1:                     /* indices 1,4,7,10,... */
            if (n == 1) return (seq_t)-1;
            return (seq_t)(-3 * ((seq_t)(n / 3)) - 2);
        case 2:                     /* indices 2,5,8,11,... */
            return (seq_t)(-((seq_t)(n / 3)) - 1);
    }
    return (seq_t)0;                 /* unreachable, keeps compiler happy */
}

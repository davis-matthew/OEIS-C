/*
 * A128615  Expansion of x/(1 + x + x^2 - x^3 - x^4 - x^5).
 * https://oeis.org/A128615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 0 if n % 3 == 0 otherwise a(n) = ((n + 2) / 3) * (1 if n
 *            % 3 == 1 else -1)
 * Range:     i8 n <= 141, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;                /* remainder mod 3 */
    if (r == 0) {
        return 0;                   /* a(0), a(3), a(6), … */
    }

    seq_t sn = (seq_t)n;            /* convert index to seq_t */
    seq_t val = (sn + 2) / 3;       /* floor division by 3 */

    if (r == 1) {
        return val;                 /* positive values */
    }
    /* r == 2 */
    return -val;                    /* negative values */
}

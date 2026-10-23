/*
 * A024031  a(n) = 3^n - n^8.
 * https://oeis.org/A024031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3^n - n^8
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 14, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 3^n via exponentiation by squaring */
    seq_t pow3 = 1;
    seq_t base3 = 3;
    idx_t e = n;
    while (e > 0)
    {
        if (e & 1)
            pow3 *= base3;
        base3 *= base3;
        e >>= 1;
    }

    /* compute n^8: (n^2)^4 */
    seq_t n8 = (seq_t)n;
    n8 *= n8; /* n^2 */
    n8 *= n8; /* n^4 */
    n8 *= n8; /* n^8 */

    return pow3 - n8;
}

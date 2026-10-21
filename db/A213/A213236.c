/*
 * A213236  a(n) = (-n)^(n-1).
 * https://oeis.org/A213236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-n)^(n-1)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Protect against n == 0 (outside the offset) */
    if (n == 0)
        return 0;

    /* Compute the negative base */
    seq_t base = -(seq_t)n;
    seq_t result = (seq_t)1;
    idx_t e = n - 1;

    /* Fast exponentiation by squaring */
    while (e > 0)
    {
        if (e & 1)
            result = result * base;
        base = base * base;
        e >>= 1;
    }
    return result;
}

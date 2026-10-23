/*
 * A165233  Signed denominators of terms in series expansion of cos(x)+sin(x).
 * https://oeis.org/A165233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, sign, easy
 * Formula:   a(n) = (-1)^floor(n/2) * n!
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    /* Compute factorial n! */
    for (idx_t k = 2; k <= n; ++k)
    {
        result = result * (seq_t)k;
    }

    /* Apply sign: (-1)^floor(n/2) */
    idx_t half = n / 2;
    if ((half & 1) != 0)
    {
        result = -result;
    }

    return result;
}

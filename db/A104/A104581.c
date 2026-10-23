/*
 * A104581  Expansion of g.f. 1/(1 + x + x^3 + x^4).
 * https://oeis.org/A104581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = floor((n+3)/3)*(-1)^n
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type and compute the floor of (n+3)/3. */
    seq_t m = ((seq_t)n + 3) / 3;
    /* Apply the sign: positive if n is even, negative if n is odd. */
    return (n & 1) ? -m : m;
}

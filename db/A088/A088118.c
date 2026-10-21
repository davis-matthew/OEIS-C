/*
 * A088118  Duplicate of A088117.
 * https://oeis.org/A088118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = floor(n/10) * ((n mod 10 + 1) mod 10)
 * Range:     i8 n <= 32, i16 n <= 32, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* quotient of n by 10 */
    idx_t k = n / 10;
    /* remainder of n modulo 10 */
    uint8_t r = n % 10;
    /* (r + 1) modulo 10, fits into 4 bits */
    seq_t t = (seq_t)((r + 1) % 10);
    /* Multiply, result never exceeds 9 * floor(n/10) */
    seq_t res = (seq_t)k * t;
    return res;
}

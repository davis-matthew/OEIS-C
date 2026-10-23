/*
 * A015862  Inverse of 1853rd cyclotomic polynomial.
 * https://oeis.org/A015862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if 0 <= (n mod 1853) <= 16, -1 if 109 <= (n mod 1853)
 *            <= 125, 0 otherwise
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t P = 1853;
    idx_t r = n % P;
    if (r <= 16ULL) return (seq_t)1;
    if (r >= 109ULL && r <= 125ULL) return (seq_t)-1;
    return (seq_t)0;
}

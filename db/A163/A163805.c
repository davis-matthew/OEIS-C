/*
 * A163805  Expansion of (1 - x) * (1 - x^6) / ((1 - x^3) * (1 - x^4)) in
 *          powers of x.
 * https://oeis.org/A163805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; For n > 0: a(n) = 0 if n is even, -1 if n ≡ 1 (mod 4),
 *            1 if n ≡ 3 (mod 4)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if ((n & 1u) == 0u)          /* even n > 0 */
        return (seq_t)0;
    /* n is odd */
    return ((n & 3u) == 1u) ? (seq_t)-1 : (seq_t)1;
}

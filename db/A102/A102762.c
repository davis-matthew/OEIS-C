/*
 * A102762  Curvatures of (largest) kissing circles along the circumference,
 *          starting with curvature = -1 and 2.
 * https://oeis.org/A102762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^2 - 4*n + 6 for n > 1, with a(0) = -1, a(1) = a(2) = 2
 * Range:     i8 n <= 13, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special initial values explicitly. */
    if (n == 0) {
        return (seq_t)-1;
    }
    if (n == 1 || n == 2) {
        return (seq_t)2;
    }

    /* For n >= 3 use the quadratic formula.  Use a wider type
       for the arithmetic to avoid overflow when seq_t is narrower
       than 64 bits.  The result fits in 64 bits for the test
       ranges (up to at least n = 10000). */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long val = nn * (nn - 4ULL) + 6ULL;
    return (seq_t)val;
}

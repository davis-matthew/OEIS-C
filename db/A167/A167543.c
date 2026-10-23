/*
 * A167543  a(n) = (n-5)*(n-6)*(n-7)*(n-16)/24.
 * https://oeis.org/A167543
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n-5)*(n-6)*(n-7)*(n-16)/24
 * Range:     i8 n <= 8, i16 n <= 22, i32 n <= 278, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t v1 = nn - (seq_t)5;
    seq_t v2 = nn - (seq_t)6;
    seq_t v3 = nn - (seq_t)7;
    seq_t v4 = nn - (seq_t)16;

    seq_t prod = v1 * v2;
    prod *= v3;
    prod *= v4;

    return prod / (seq_t)24;
}

/*
 * A141214  Defining A to be the interior angle of a regular polygon, the
 *          number of constructible regular polygons such that A is in a field
 *          extension <= degree 2^n, starting with n=0. This is also the
 *          number of values of x such that phi(x)/2 is a power of 2 <= 2^n
 *          (where phi is Euler's phi function), also s...
 * https://oeis.org/A141214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)(n+6)/2 for n <= 31, 34*n - 462 for n >= 31
 * Range:     u8 n <= 19, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    if (n <= 31) {
        return (tn + 1) * (tn + 6) / 2;
    } else {
        return 34 * tn - 462;
    }
}

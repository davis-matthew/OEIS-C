/*
 * A000023  a(n) is the number of permutations on {1,2,...,n} with an even
 *          number of fixed points minus the number of permutations with an
 *          odd number of fixed points.
 * https://oeis.org/A000023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*a(n-1) + (-2)^n, a(0) = 1
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;
    seq_t p = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        p *= -2;
        r = r * (seq_t)k + p;
    }
    return r;
}

/*
 * A106252  Number of positive integer triples (x,y,z), with x<=y<=z<=n, such
 *          that each of x,y and z divides the sum of the other two.
 * https://oeis.org/A106252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n/2) + floor(n/3)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n >= 1; undefined behaviour for n == 0, but return 0 for safety. */
    if (n == 0)
        return 0;
    return (seq_t)n + (seq_t)(n / 2) + (seq_t)(n / 3);
}

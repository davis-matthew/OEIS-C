/*
 * A057353  a(n) = floor(3n/4).
 * https://oeis.org/A057353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3*n/4)
 * Range:     u8 n <= 341, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use division to avoid overflow: a(n) = 3*(n/4) + floor(3*(n%4)/4) */
    static const int rem[4] = {0, 0, 1, 2};
    idx_t q = n / 4;          /* quotient n/4  */
    int r = n % 4;            /* remainder n%4  */
    return (seq_t)(3 * q + rem[r]);
}

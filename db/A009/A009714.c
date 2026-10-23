/*
 * A009714  a(n) = Product_{i=0..8} floor((n+i)/9).
 * https://oeis.org/A009714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=0..8} floor((n+i)/9)
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 105, u64 n <= 1244,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 9 all floors are zero */
    if (n < 9)
        return 0;

    /* Let q = floor(n / 9) and j = n mod 9 */
    seq_t q = (seq_t)(n / 9);
    idx_t j = n % 9;

    /* a(9*q + j) = q^(9-j) * (q+1)^j */
    seq_t result = 1;

    for (idx_t i = 0; i < (9 - j); ++i)
        result = result * q;

    seq_t r = q + 1;
    for (idx_t i = 0; i < j; ++i)
        result = result * r;

    return result;
}

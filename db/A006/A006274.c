/*
 * A006274  First differences of A006268.
 * https://oeis.org/A006274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = b(n)^3 + 2*b(n), where b(n) = A006268(n), b(0) = 3,
 *            b(n+1) = b(n)^3 + 3*b(n); i.e. a(n) = b(n+1) - b(n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t b = 3;
    idx_t i;
    for (i = 0; i < n; i++)
        b = b * b * b + 3 * b;
    return b * b * b + 2 * b;
}

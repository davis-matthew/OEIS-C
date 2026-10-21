/*
 * A098749  Let f(n) = ((n^4-n^3-1)/(n^3-n-1))^2; then a(n) = floor(f(n)).
 * https://oeis.org/A098749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 1 for n = 0,1,2; a(n) = (n-1)^2 + 1 for n >= 3
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)1;

    seq_t nn = n;
    seq_t tmp = nn - 1;
    return tmp * tmp + 1;
}

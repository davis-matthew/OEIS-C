/*
 * A002276  a(n) = 2*(10^n - 1)/9.
 * https://oeis.org/A002276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + 2, a(0) = 0 (i.e. a(n) = 2*(10^n - 1)/9)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 0;
    idx_t i;
    for (i = 0; i < n; i++)
        r = r * 10 + 2;
    return r;
}

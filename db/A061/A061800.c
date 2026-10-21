/*
 * A061800  a(n) = n + (-1)^(n mod 3).
 * https://oeis.org/A061800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + (-1)^(n mod 3)
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    seq_t res = (seq_t)n;
    if (r == 1)
        res -= 1;
    else
        res += 1;
    return res;
}

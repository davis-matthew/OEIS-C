/*
 * A100402  Digital root of 4^n.
 * https://oeis.org/A100402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 4^n mod 9 (period 3: 1,4,7)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    if (r == 0)
        return 1;
    else if (r == 1)
        return 4;
    else
        return 7;
}

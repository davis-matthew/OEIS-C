/*
 * A052539  a(n) = 4^n + 1.
 * https://oeis.org/A052539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)4;
    idx_t  exp   = n;
    while (exp)
    {
        if (exp & 1UL)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result + (seq_t)1;
}

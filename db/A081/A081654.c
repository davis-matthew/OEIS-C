/*
 * A081654  a(n) = 2*4^n - 0^n.
 * https://oeis.org/A081654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*4^n for n > 0, a(0) = 1 (0^0 is taken as 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t pow4 = (seq_t)1;   /* will hold 4^n */
    seq_t base = (seq_t)4;   /* base for squaring */
    idx_t  exp  = n;

    /* binary exponentiation to compute 4^n */
    while (exp)
    {
        if (exp & 1)
            pow4 *= base;
        base *= base;
        exp >>= 1;
    }

    /* a(n) = 2 * 4^n */
    return pow4 * (seq_t)2;
}

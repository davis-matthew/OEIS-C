/*
 * A074622  a(n) = 7^n + 8^n.
 * https://oeis.org/A074622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 7^n and 8^n by repeated small‑factor multiplication */
    seq_t pow7 = 1;
    seq_t pow8 = 1;

    /* loop n times; for n = 0 the loop body is skipped and both powers stay 1 */
    for (idx_t i = 0; i < n; ++i)
    {
        pow7 *= (seq_t)7;
        pow8 *= (seq_t)8;
    }

    return pow7 + pow8;
}

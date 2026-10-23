/*
 * A155627  a(n) = 6^n - 4^n + 1.
 * https://oeis.org/A155627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - 4^n + 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow6 = 1;
    seq_t base6 = 6;
    idx_t exp = n;
    while (exp)
    {
        if (exp & 1U)
            pow6 = pow6 * base6;
        base6 = base6 * base6;
        exp >>= 1;
    }

    seq_t pow4 = 1;
    seq_t base4 = 4;
    exp = n;
    while (exp)
    {
        if (exp & 1U)
            pow4 = pow4 * base4;
        base4 = base4 * base4;
        exp >>= 1;
    }

    return pow6 - pow4 + (seq_t)1;
}

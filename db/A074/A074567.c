/*
 * A074567  a(n) = 4^n + 6^n + 9^n.
 * https://oeis.org/A074567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 6^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & (idx_t)1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p4 = pow_seq((seq_t)4, n);
    seq_t p6 = pow_seq((seq_t)6, n);
    seq_t p9 = pow_seq((seq_t)9, n);
    return p4 + p6 + p9;
}

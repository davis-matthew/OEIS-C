/*
 * A074513  a(n) = 1^n + 4^n + 7^n.
 * https://oeis.org/A074513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 4^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast binary exponentiation for seq_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t cur = base;
    idx_t e = exp;
    while (e)
    {
        if (e & 1) result = result * cur;
        e >>= 1;
        if (e) cur = cur * cur;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p4 = pow_seq((seq_t)4, n);
    seq_t p7 = pow_seq((seq_t)7, n);
    return (seq_t)1 + p4 + p7;
}

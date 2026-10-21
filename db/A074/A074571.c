/*
 * A074571  a(n) = 5^n + 6^n + 7^n.
 * https://oeis.org/A074571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n + 6^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute integer power using exponentiation by squaring.
   Works for any seq_t implementation that supports * and + operators. */
static seq_t int_pow_seq(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    seq_t cur = base;
    while (exp)
    {
        if (exp & 1U)
            res = res * cur;
        cur = cur * cur;
        exp >>= 1U;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t p5 = int_pow_seq((seq_t)5, n);
    seq_t p6 = int_pow_seq((seq_t)6, n);
    seq_t p7 = int_pow_seq((seq_t)7, n);
    return p5 + p6 + p7;
}

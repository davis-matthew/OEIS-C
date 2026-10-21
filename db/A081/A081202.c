/*
 * A081202  8th binomial transform of (0,1,0,1,0,1,....), A000035.
 * https://oeis.org/A081202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (9^n - 7^n) / 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp != 0)
    {
        if (exp & 1U)
            res *= base;
        base *= base;
        exp >>= 1U;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t pow9 = pow_seq(9, n);
    seq_t pow7 = pow_seq(7, n);
    return (pow9 - pow7) / 2;
}

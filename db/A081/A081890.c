/*
 * A081890  a(n) = 9^n - 8^n - 7^n - 6^n + 3*5^n.
 * https://oeis.org/A081890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9^n - 8^n - 7^n - 6^n + 3 * 5^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* helper: fast binary exponentiation of a seq_t by an idx_t */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp)
    {
        if (exp & 1)
            res *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t p5 = pow_seq_t((seq_t)5, n);
    seq_t p6 = pow_seq_t((seq_t)6, n);
    seq_t p7 = pow_seq_t((seq_t)7, n);
    seq_t p8 = pow_seq_t((seq_t)8, n);
    seq_t p9 = pow_seq_t((seq_t)9, n);

    seq_t result = p9;
    result -= p8;
    result -= p7;
    result -= p6;
    result += p5 * (seq_t)3;

    return result;
}

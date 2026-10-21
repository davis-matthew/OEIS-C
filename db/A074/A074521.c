/*
 * A074521  a(n) = 1^n + 6^n + 8^n.
 * https://oeis.org/A074521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 6^n + 8^n Computed via binary exponentiation
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow6 = pow_seq_t((seq_t)6, n);
    seq_t pow8 = pow_seq_t((seq_t)8, n);
    return pow6 + pow8 + (seq_t)1;
}

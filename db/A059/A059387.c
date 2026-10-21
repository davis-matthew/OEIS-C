/*
 * A059387  Jordan function J_n(6) (see A059379).
 * https://oeis.org/A059387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n - 1) * (3^n - 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t two_n   = pow_seq((seq_t)2, n);
    seq_t three_n = pow_seq((seq_t)3, n);
    two_n   -= (seq_t)1;
    three_n -= (seq_t)1;
    return two_n * three_n;
}

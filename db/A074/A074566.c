/*
 * A074566  a(n) = 4^n + 6^n + 8^n.
 * https://oeis.org/A074566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 6^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;   /* integer literals are fine with seq_t */
    while (exp)
    {
        if (exp & 1u)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    return pow_seq((seq_t)4, n) + pow_seq((seq_t)6, n) + pow_seq((seq_t)8, n);
}

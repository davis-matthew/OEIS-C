/*
 * A061252  a(n) = 16^n - 15^n.
 * https://oeis.org/A061252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^n - 15^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1u)
            result *= base;
        exp >>= 1u;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;
    seq_t pow16 = pow_uint((seq_t)16, n);
    seq_t pow15 = pow_uint((seq_t)15, n);
    return pow16 - pow15;
}

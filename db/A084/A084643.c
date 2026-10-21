/*
 * A084643  a(n) = 3^(n-1)*(2*n-3) + 2^(n+1).
 * https://oeis.org/A084643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-1)*(2*n-3) + 2^(n+1) (offset 0)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1U) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* handle the offset explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;   /* 3^(0)*(-1) + 2^2 = 3 */

    /* n >= 2 : the multiplier (2*n-3) is positive */
    seq_t pow3 = pow_seq_t((seq_t)3, n - 1);
    seq_t pow2 = pow_seq_t((seq_t)2, n + 1);
    seq_t mult = (seq_t)(2 * n - 3);

    return pow3 * mult + pow2;
}

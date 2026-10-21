/*
 * A055276  First differences of 11^n (A001020).
 * https://oeis.org/A055276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(n) = 10 * 11^(n-1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
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
    if (n == 0)
        return 1;                      /* a(0) = 1 */
    seq_t p = pow_seq_t(11, n - 1);     /* 11^(n-1) */
    return 10 * p;                     /* 10 * 11^(n-1) */
}

/*
 * A081683  a(n) = (10^n - 9^n - 8^n - 7^n + 4*6^n)/2.
 * https://oeis.org/A081683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n - 9^n - 8^n - 7^n + 4*6^n)/2, offset 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_int(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp > 0)
    {
        if (exp & 1)
            result *= cur;
        if (exp > 1)
            cur *= cur;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p10 = pow_int(10, n);
    seq_t p9  = pow_int(9,  n);
    seq_t p8  = pow_int(8,  n);
    seq_t p7  = pow_int(7,  n);
    seq_t p6  = pow_int(6,  n);
    seq_t numerator = p10 - p9 - p8 - p7 + (p6 * 4);
    return numerator / 2;
}

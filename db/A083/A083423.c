/*
 * A083423  a(n) = (5*3^n + (-3)^n)/6.
 * https://oeis.org/A083423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5*3^n + (-3)^n)/6 Simplified: a(2k) = 9^k, a(2k+1) =
 *            2*9^k
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
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
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t k = n >> 1;
    seq_t res = pow_seq_t((seq_t)9, k);
    if (n & 1)
        res = res * (seq_t)2;
    return res;
}

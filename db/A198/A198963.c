/*
 * A198963  a(n) = 6*9^n-1.
 * https://oeis.org/A198963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*9^n-1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;

    seq_t pow9 = 1;
    seq_t base = 9;
    idx_t exp = n;
    while (exp)
    {
        if (exp & 1ULL) pow9 *= base;
        base *= base;
        exp >>= 1;
    }
    seq_t res = pow9 * 6;
    res -= 1;
    return res;
}

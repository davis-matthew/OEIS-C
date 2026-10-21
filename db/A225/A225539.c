/*
 * A225539  Numbers n where 2^n and n have the same digital root.
 * https://oeis.org/A225539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*n - 3 + (-1)^n
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)9 * (seq_t)n - (seq_t)3;
    if ((n & 1ULL) == 0)
        res += (seq_t)1;
    else
        res -= (seq_t)1;
    return res;
}

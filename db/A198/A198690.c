/*
 * A198690  a(n) = 9*7^n-1.
 * https://oeis.org/A198690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*7^n-1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)7;
    idx_t e = n;
    while (e) {
        if (e & (idx_t)1) result *= base;
        base *= base;
        e >>= 1;
    }
    result *= (seq_t)9;
    result -= (seq_t)1;
    return result;
}

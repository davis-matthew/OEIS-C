/*
 * A036291  a(n) = n*5^n.
 * https://oeis.org/A036291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*5^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t pow5 = 1;
    seq_t base = 5;
    idx_t e = n;
    while (e) {
        if (e & 1ULL)
            pow5 *= base;
        base *= base;
        e >>= 1;
    }
    return pow5 * (seq_t)n;
}

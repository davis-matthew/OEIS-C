/*
 * A018637  Divisors of 772.
 * https://oeis.org/A018637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 772 in ascending order
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 772 = 2^2 * 193^1 */
    if (n == 0)
        return 0;
    if (n <= 3) {
        seq_t v = 1;
        idx_t i;
        for (i = 0; i < n - 1; ++i)
            v *= (seq_t)2;
        return v;
    }
    if (n <= 6) {
        seq_t v = (seq_t)193;
        idx_t i;
        for (i = 0; i < n - 4; ++i)
            v *= (seq_t)2;
        return v;
    }
    return 0;   /* n > number of divisors */
}

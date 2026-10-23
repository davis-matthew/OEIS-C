/*
 * A018625  Divisors of 748.
 * https://oeis.org/A018625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth smallest divisor of 748 (ascending)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 748 = 2^2 * 11 * 17; there are 12 divisors. */
    static const seq_t divisors[12] = {
        1, 2, 4, 11, 17, 22, 34, 44, 68, 187, 374, 748
    };
    if (n < 1 || n > 12) {
        /* For indices outside the range of divisors, return 0. */
        return (seq_t)0;
    }
    return divisors[n - 1];
}

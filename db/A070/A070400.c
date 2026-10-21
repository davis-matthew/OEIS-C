/*
 * A070400  a(n) = 6^n mod 37.
 * https://oeis.org/A070400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n mod 37
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Period of 4: 6^0=1, 6^1=6, 6^2=36, 6^3=31 */
    static const seq_t period[4] = {
        (seq_t)1,
        (seq_t)6,
        (seq_t)36,
        (seq_t)31
    };
    return period[n & 3u];
}

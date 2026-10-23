/*
 * A037902  a(n) = (greatest base 8 digit of n) - (least base 8 digit of n).
 * https://oeis.org/A037902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (greatest base 8 digit of n) – (least base 8 digit of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n == 0 the only digit is 0, hence the result is 0. */
    if (n == 0)
        return (seq_t)0;

    idx_t max_d = 0, min_d = 7;   /* 0 ≤ digit ≤ 7 */
    idx_t m = n;
    while (m > 0) {
        idx_t d = m % 8;
        if (d > max_d) max_d = d;
        if (d < min_d) min_d = d;
        m /= 8;
    }
    return (seq_t)(max_d - min_d);
}

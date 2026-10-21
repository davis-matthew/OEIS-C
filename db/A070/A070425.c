/*
 * A070425  a(n) = 7^n mod 43.
 * https://oeis.org/A070425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 43 (period 6)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[6] = {
        (seq_t)1,  (seq_t)7,  (seq_t)6,
        (seq_t)42, (seq_t)36, (seq_t)37
    };
    return period[n % 6];
}

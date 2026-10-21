/*
 * A226044  Period of length 8: 1, 64, 16, 64, 4, 64, 16, 64.
 * https://oeis.org/A226044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A205383(n+7)^2. The resulting sequence is periodic with
 *            period 8: 1, 64, 16, 64, 4, 64, 16, 64
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 8‑element period array */
    static const seq_t period[8] = {
        (seq_t)1,
        (seq_t)64,
        (seq_t)16,
        (seq_t)64,
        (seq_t)4,
        (seq_t)64,
        (seq_t)16,
        (seq_t)64
    };
    return period[n & 7];   /* n mod 8 – works for any idx_t */
}

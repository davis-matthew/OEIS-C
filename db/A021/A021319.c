/*
 * A021319  Decimal expansion of 1/315.
 * https://oeis.org/A021319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A021319: decimal expansion of 1/315.
    a(0) = 0
    For n > 0, the digits repeat every 6 positions:
        [0, 3, 1, 7, 4, 6]
    Hence a(n) = period[(n-1) % 6]  (n >= 1)                                           */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Periodic pattern starting from n = 1 */
    static const seq_t period[6] = {0, 3, 1, 7, 4, 6};
    return period[(n - 1) % 6];
}

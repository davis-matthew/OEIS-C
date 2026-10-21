/*
 * A073583  Decimal expansion of 23/19.
 * https://oeis.org/A073583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th digit of the decimal expansion of 23/19, with
 *            a(1) = 1 (the integer part). After the decimal point the
 *            repeating block is 2,1,0,5,2,6,3,1,5,7,8,9,4,7,3,6,8,4 (period
 *            18)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;            /* integer part */

    /* Periodic digits after the decimal point */
    static const seq_t period[18] = { 2, 1, 0, 5, 2, 6, 3, 1,
                                      5, 7, 8, 9, 4, 7, 3, 6, 8, 4 };

    /* n >= 2: map to 0..17 */
    idx_t idx = (n - 2) % 18;
    return period[idx];
}

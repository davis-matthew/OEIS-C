/*
 * A021256  Decimal expansion of 1/252.
 * https://oeis.org/A021256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   digits of the decimal expansion of 1/252. a(n) = first[n] for 0
 *            ≤ n < 6, a(n) = period[n mod 6] for n ≥ 6, where first =
 *            {0,0,3,9,6,8} and period = {2,5,3,9,6,8}
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* digits before the periodic part */
    static const int first[6] = {0, 0, 3, 9, 6, 8};
    /* periodic digits after index 5 (period 6) */
    static const int period[6] = {2, 5, 3, 9, 6, 8};

    if (n < 6) {
        return (seq_t)first[(idx_t)n];
    } else {
        return (seq_t)period[(idx_t)(n % 6)];
    }
}

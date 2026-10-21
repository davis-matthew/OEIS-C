/*
 * A070365  a(n) = 5^n mod 7.
 * https://oeis.org/A070365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 7, periodic with period 6
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence values repeat every 6 indices: [1, 5, 4, 6, 2, 3] */
    static const seq_t period[6] = {1, 5, 4, 6, 2, 3};
    return period[n % 6];
}

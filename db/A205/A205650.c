/*
 * A205650  Period 12: repeat (1, 6, 8, 6, 1, 9, 8, 3, 1, 3, 8, 9).
 * https://oeis.org/A205650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1,6,8,6,1,9,8,3,1,3,8,9)[ (n-1) mod 12 ]
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period 12: repeat (1, 6, 8, 6, 1, 9, 8, 3, 1, 3, 8, 9). */

seq_t a(idx_t n)
{
    static const seq_t period[12] = {1, 6, 8, 6, 1, 9, 8, 3, 1, 3, 8, 9};
    /* Convert n to zero-based index; n is guaranteed >= 1 in the spec. */
    idx_t idx = (n - 1) % 12;
    return period[idx];
}

/*
 * A131707  Period 12: repeat 1, 1, 3, 7, 7, 1, 9, 9, 7, 3, 3, 9 .
 * https://oeis.org/A131707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001333(n) mod 10. The sequence is periodic with period
 *            12: 1, 1, 3, 7, 7, 1, 9, 9, 7, 3, 3, 9, 1, …
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[12] = {1, 1, 3, 7, 7, 1, 9, 9, 7, 3, 3, 9};
    return period[n % 12];
}

/*
 * A132400  Period 4: repeat [1, 5, 3, 1].
 * https://oeis.org/A132400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) for n >= 4; initial values: a(0) = 1, a(1) = 5,
 *            a(2) = 3, a(3) = 1
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n & 3U) {
        case 0: return 1;
        case 1: return 5;
        case 2: return 3;
        default: return 1;  /* case 3 */
    }
}

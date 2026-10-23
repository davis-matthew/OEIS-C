/*
 * A131369  Period 10: repeat [5, 4, 5, 4, 3, 4, 5, 4, 5, 0].
 * https://oeis.org/A131369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5,4,5,4,3,4,5,4,5,0 repeated periodically (period 10),
 *            i.e. a(n) = pattern[n mod 10] with pattern[0..9] as above
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* static table of one period */
    static const seq_t pattern[10] = {5, 4, 5, 4, 3, 4, 5, 4, 5, 0};

    /* compute index modulo 10 */
    idx_t idx = n % 10;
    return pattern[idx];
}

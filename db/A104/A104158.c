/*
 * A104158  Numbers on a standard, London, or clock dartboard read in a
 *          counterclockwise direction.
 * https://oeis.org/A104158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number at position n (starting from 0) when the numbers
 *            1–20 are arranged on a standard dartboard in clockwise order
 *            20,1,18,4,13,6,10,15,2,17,3,19,7,16,8,11,14,9,12,5. Then a(n) is
 *            read counter‑clockwise, i.e. a(n) = cw[(20−(n % 20)) % 20]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Standard clockwise numbering of a 20‑sector dartboard.  */
    static const seq_t cw[20] = {
        20, 1, 18, 4, 13, 6, 10, 15, 2, 17,
        3, 19, 7, 16, 8, 11, 14, 9, 12, 5
    };
    idx_t k = n % 20;            /* position in the period   */
    idx_t idx = (20 - k) % 20;   /* reverse direction       */
    return cw[idx];
}

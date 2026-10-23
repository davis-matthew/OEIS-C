/*
 * A133143  Maximal number of mutually nonattacking Super Queens on an n X n
 *          board. (A Super Queen is a queen with both queen and knight
 *          powers.)
 * https://oeis.org/A133143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = {1,1,1,2,4,4,5,6,8} for n=1..9; a(n) = n for n ≥ 10
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small values for n = 1..9 */
    static const seq_t small[10] = {0, 1, 1, 1, 2, 4, 4, 5, 6, 8};

    if (n <= 9)
        return small[n];
    return (seq_t)n;
}

/*
 * A076119  Every second sector of a dartboard, starting at the top (20) and
 *          working around clockwise.
 * https://oeis.org/A076119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = dartboard[ ((n-1)*2) mod 20 ], period 10
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Standard dartboard numbering clockwise */
    static const seq_t dartboard[20] = {
        20, 1, 18, 4, 13, 6,
        10, 15, 2, 17, 3, 19,
        7, 16, 8, 11, 14, 9,
        12, 5
    };

    /* n is unsigned and >= 1, so (n-1) is safe.
       Compute the index of the desired sector and return its value. */
    uint64_t pos = ((uint64_t)(n - 1) * 2ULL) % 20ULL;
    return dartboard[pos];
}

/*
 * A225551  Longest checkmate in king and queen versus king endgame on an n X
 *          n chessboard.
 * https://oeis.org/A225551
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - 3 - floor(n/2) for n > 8; defined for n >=3
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _A225551_SMALL[6] = {1, 4, 6, 7, 8, 10};

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;          /* undefined below offset, return 0 */
    if (n <= 8)
        return _A225551_SMALL[n - 3];

    seq_t res = (seq_t)n * 2;
    res -= 3;
    res -= (seq_t)(n / 2);
    return res;
}

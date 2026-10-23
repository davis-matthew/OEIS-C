/*
 * A104519  Sufficient number of monominoes to exclude X-pentominoes from an n
 *          X n board.
 * https://oeis.org/A104519
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^2 / 5) - 4 for n > 15 * Exceptions: * a(3) = 1,
 *            a(4) = 2, a(5) = 3, a(6) = 4, a(7) = 7, * a(8) = 10, and a(15) =
 *            40. * The sequence is defined only for n ≥ 3; values for n < 3
 *            are returned as 0
 * Range:     u8 n <= 15, u16 n <= 461, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle out-of-offset values */
    if (n < 3)
        return (seq_t)0;

    /* explicit small cases and the unique 15th term */
    switch (n) {
    case 3:  return (seq_t)1;
    case 4:  return (seq_t)2;
    case 5:  return (seq_t)3;
    case 6:  return (seq_t)4;
    case 7:  return (seq_t)7;
    case 8:  return (seq_t)10;
    case 15: return (seq_t)40;
    default:
        /* compute floor(n^2 / 5) - 4 */
        seq_t nn = (seq_t)n;
        seq_t sq = nn * nn;       /* n*n fits comfortably in seq_t for expected ranges */
        return (sq / (seq_t)5) - (seq_t)4;
    }
}

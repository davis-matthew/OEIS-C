/*
 * A172222  Number of ways to place 4 nonattacking zebras on a 4 X n board.
 * https://oeis.org/A172222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(8*n^4 - 48*n^3 + 202*n^2 - 471*n + 507)/3 for n ≥ 9
 *            (for n < 9 the values are those given in the OEIS description)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 115, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _first[9] = {0, 1, 70, 406, 1168, 2948, 6576, 13122, 23808};

seq_t a(idx_t n)
{
    if (n < 9)
        return _first[n];

    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;          /* n²   */
    seq_t n3  = n2 * nn;           /* n³   */
    seq_t n4  = n3 * nn;           /* n⁴   */

    seq_t t = 8 * n4 - 48 * n3 + 202 * n2 - 471 * nn + 507;

    return 4 * t / 3;
}

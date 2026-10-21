/*
 * A228679  Number of nX4 binary arrays with no two ones adjacent
 *          horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 2*a(n-2) - 8*a(n-3). Initial values: a(1)=8,
 *            a(2)=40, a(3)=216
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for offset 1, return 0 for safety */
        return (seq_t)0;
    if (n == 1)           return (seq_t)8;
    if (n == 2)           return (seq_t)40;
    if (n == 3)           return (seq_t)216;

    seq_t a1 = (seq_t)8;
    seq_t a2 = (seq_t)40;
    seq_t a3 = (seq_t)216;
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)(6 * a3 - 2 * a2 - 8 * a1);
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }
    return cur;
}

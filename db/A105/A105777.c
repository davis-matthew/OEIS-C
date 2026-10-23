/*
 * A105777  Trajectory of 1 under the morphism 1->{1,2,2,2,1}, 2->{4,3,3,3,4},
 *          3->{2,1,1,1,2}, 4->{3,4,4,4,3}.
 * https://oeis.org/A105777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, let q=n/5, r=n%5; a(n)=expand[a(q)][r]. The
 *            expansion table is 1 → {1,2,2,2,1} 2 → {4,3,3,3,4} 3 →
 *            {2,1,1,1,2} 4 → {3,4,4,4,3}
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    static const seq_t expand[5][5] = {
        {0, 0, 0, 0, 0},          /* unused row for symbol 0 */
        {1, 2, 2, 2, 1},          /* 1 → 1 2 2 2 1 */
        {4, 3, 3, 3, 4},          /* 2 → 4 3 3 3 4 */
        {2, 1, 1, 1, 2},          /* 3 → 2 1 1 1 2 */
        {3, 4, 4, 4, 3}           /* 4 → 3 4 4 4 3 */
    };

    idx_t q = n / 5;
    idx_t r = n % 5;
    seq_t parent = a(q);
    return expand[(unsigned)parent][(unsigned)r];
}

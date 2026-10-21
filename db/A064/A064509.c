/*
 * A064509  Marks (in fathoms) on lead line used by ships on the Mississippi
 *          River.
 * https://oeis.org/A064509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 5(n-5) for n ≥ 9; for 1 ≤ n ≤ 8 the terms are
 *            {2,3,5,7,10,13,15,17}
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* predefined values for n = 1..8 */
    static const seq_t first[8] = {2, 3, 5, 7, 10, 13, 15, 17};

    if (n <= 8)
        return first[n - 1];

    /* for n ≥ 9: a(n) = 5*(n-5) = 5*n - 25 */
    seq_t nn = (seq_t)n;
    return 5 * nn - (seq_t)25;
}

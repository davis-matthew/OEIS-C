/*
 * A076095  Initial terms of rows in A076099.
 * https://oeis.org/A076095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 - 3n + 10)/2 for n >= 7 (first six terms are 1, 2,
 *            3, 5, 9, 13)
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n <= 6) {
        static const seq_t tbl[6] = {1, 2, 3, 5, 9, 13};
        return tbl[n-1];
    }
    idx_t k = n;
    return (seq_t)((k * k - 3 * k + 10) / 2);
}

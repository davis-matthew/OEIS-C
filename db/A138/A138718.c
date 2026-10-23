/*
 * A138718  Group number of the elements of the n-th column of the periodic
 *          table of the elements with 18 columns.
 * https://oeis.org/A138718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(n) = group number of column n in the periodic table (period
 *            18)
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[18] = {
        1, 2, 3, 4, 5, 6, 7, 8,   /* columns  1–8 */
        8, 8,                       /* columns  9–10 */
        1, 2,                       /* columns 11–12 */
        3, 4, 5, 6, 7, 8            /* columns 13–18 */
    };
    return tbl[(n - 1) % 18];
}

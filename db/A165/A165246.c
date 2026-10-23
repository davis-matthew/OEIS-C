/*
 * A165246  a(n) = (10^n + 53)/9.
 * https://oeis.org/A165246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n + 53)/9
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                 /* undefined for the offset 1 but harmless */
    }
    seq_t val = 0;
    for (idx_t i = 1; i < n; ++i) {
        val = val * 10 + 1;       /* build the leading (n-1) ones */
    }
    val = val * 10 + 7;           /* append the final seven */
    return val;
}

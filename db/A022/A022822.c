/*
 * A022822  a(n) = [ (n+2)/(n-1) ] + [ (n+4)/(n-2) ] + ... + [ (3n-2)/1 ].
 * https://oeis.org/A022822
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+2)/(n-1)) + floor((n+4)/(n-2)) + ... +
 *            floor((3n-2)/1)
 * Range:     u8 n <= 27, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) {
        /* behaviour below the offset is irrelevant */
        return 0;
    }
    seq_t sum = 0;
    for (idx_t i = 1; i < n; ++i) {
        idx_t num = n + ((idx_t)2 * i);
        idx_t den = n - i;
        seq_t term = (seq_t)(num / den);
        sum += term;
    }
    return sum;
}

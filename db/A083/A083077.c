/*
 * A083077  Fifth row of number array A083075.
 * https://oeis.org/A083077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n)=11*a(n-1)-2 */
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    seq_t cur = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        cur = cur * (seq_t)11 - (seq_t)2;
    }
    return cur;
}

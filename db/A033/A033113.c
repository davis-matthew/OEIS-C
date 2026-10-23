/*
 * A033113  Base-3 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0.
 * https://oeis.org/A033113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1; for n>=2, a(n) = 3 * a(n-1) + (n is odd ? 1 : 0)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but defensive */
    seq_t res = 1;                 /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        res = res * (seq_t)3 + (seq_t)(i & 1u);
    }
    return res;
}

/*
 * A037544  Base-3 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,2.
 * https://oeis.org/A037544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1, a(n)=a(n-1)*3 + (n%3==1?1:2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* behaviour below the offset is irrelevant */
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        val = val * 3 + ((i % 3) == 1 ? (seq_t)1 : (seq_t)2);
    }
    return val;
}

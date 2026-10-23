/*
 * A117614  a(0)=1, a(n)=4a(n-1)+2 for n odd, a(n)=4a(n-1) for n even.
 * https://oeis.org/A117614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0: a(n)=4*a(n-1)+2 if n odd, else a(n)=4*a(n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;               /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        val = val * 4;
        if (i & 1)               /* i is odd */
            val = val + 2;
    }
    return val;
}

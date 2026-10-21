/*
 * A078904  a(n) = 4a(n-1) + 3n with a(0) = 0.
 * https://oeis.org/A078904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3)*(4^(n+1) - 3*n - 4) (recurrence: a(0)=0,
 *            a(n)=4*a(n-1)+3*n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        val = val * 4 + 3 * (seq_t)i;
    }
    return val;
}

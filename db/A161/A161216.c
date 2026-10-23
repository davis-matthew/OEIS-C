/*
 * A161216  a(n) = A161210(n)*4.
 * https://oeis.org/A161216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(n) = 4 * n*(n+1)/2 for n = 0,1,2; a(n) = 28 * (n-2) for n >= 3
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    if (x <= 2) {
        return (seq_t)4 * (x * (x + 1) / 2);
    } else {
        return (seq_t)28 * (x - 2);
    }
}

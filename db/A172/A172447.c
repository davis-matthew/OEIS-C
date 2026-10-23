/*
 * A172447  a(n) = (-1 + 5*2^(2*n + 1) - 3*n)/9.
 * https://oeis.org/A172447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = 4*a(n) + n (a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;                       /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        val = val * (seq_t)4 + (seq_t)i; /* a(i+1) = 4*a(i) + i */
    }
    return val;
}

/*
 * A000280  a(n) = a(n-1) + a(n-2)^3.
 * https://oeis.org/A000280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2)^3, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p = 0; /* a(i-2) */
    seq_t q = 1; /* a(i-1) */
    idx_t i;
    if (n == 0)
        return p;
    for (i = 2; i <= n; i++) {
        seq_t r = q + p * p * p;
        p = q;
        q = r;
    }
    return q;
}

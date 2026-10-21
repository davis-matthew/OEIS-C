/*
 * A056543  a(n) = n*a(n-1) - 1 with a(1)=1.
 * https://oeis.org/A056543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1; a(n) = n * a(n-1) - 1 (n ≥ 2)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = 1;                        /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        cur = ((seq_t)i) * cur - 1;       /* a(i) = i * a(i-1) - 1 */
    }
    return cur;
}

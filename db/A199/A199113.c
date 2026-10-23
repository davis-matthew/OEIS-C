/*
 * A199113  a(n) = (11*3^n+1)/2.
 * https://oeis.org/A199113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*3^n + 1)/2 (equivalently a(0)=6, a(n)=3*a(n-1)-1)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 6;                /* a(0) */
    if (n == 0) return val;

    for (idx_t i = 1; i <= n; ++i) {
        val = 3 * val - 1;        /* recurrence */
    }
    return val;
}

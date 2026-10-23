/*
 * A188167  a(2+4*n)=1+2*n, otherwise a(n)=2*n.
 * https://oeis.org/A188167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n % 4 == 2) ? 1 + 2 * ((n - 2) / 4) : 2 * n
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Check if n ≡ 2 (mod 4) */
    if ((n & 3) == 2) {
        /* n = 2 + 4 * m  →  m = (n - 2) / 4 */
        seq_t m = (seq_t)((n - 2) / 4);
        return 1 + 2 * m;
    } else {
        /* All other indices have value 2 * n */
        return 2 * (seq_t)n;
    }
}

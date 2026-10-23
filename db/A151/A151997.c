/*
 * A151997  a(n) = (n 1's followed by a 3)^2.
 * https://oeis.org/A151997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) = ((10^(n+1)+17)/9)^2 (equivalently, v_0 = 3, v_n =
 *            10*v_{n-1} – 17, a(n) = v_n^2 )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* v_n: number consisting of n 1's followed by a 3 */
    seq_t v = (seq_t)3;            /* v_0 = 3 */

    for (idx_t i = 1; i <= n; ++i) {
        v = (seq_t)10 * v - (seq_t)17;   /* v_i = 10*v_{i-1} - 17 */
    }

    return v * v;                  /* a(n) = v_n^2 */
}

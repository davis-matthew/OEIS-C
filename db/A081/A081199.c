/*
 * A081199  5th binomial transform of (0,1,0,1,...), A000035.
 * https://oeis.org/A081199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 24*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_prev = (seq_t)0;     /* a(0) */
    seq_t a_curr = (seq_t)1;     /* a(1) */
    seq_t a_next;

    for (idx_t i = 2; i <= n; ++i) {
        a_next = (a_curr * (seq_t)10) - (a_prev * (seq_t)24);
        a_prev = a_curr;
        a_curr = a_next;
    }
    return a_curr;
}

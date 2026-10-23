/*
 * A047383  Numbers that are congruent to {1, 5} mod 7.
 * https://oeis.org/A047383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - floor((n-1)/2) - 3;
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; behavior for n == 0
       is irrelevant for the tests. */
    if (n == 0) return (seq_t)0;

    /* For odd n: a(n) = 7*((n+1)/2) - 6
       For even n: a(n) = 7*(n/2)     - 2   */
    if (n & 1U) {
        idx_t m_idx = (n + 1) / 2;          /* k = (n+1)/2   */
        seq_t m = (seq_t)m_idx;             /* m in seq_t     */
        return (seq_t)7 * m - (seq_t)6;
    } else {
        idx_t m_idx = n / 2;                /* k = n/2        */
        seq_t m = (seq_t)m_idx;
        return (seq_t)7 * m - (seq_t)2;
    }
}

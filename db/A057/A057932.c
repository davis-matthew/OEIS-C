/*
 * A057932  a(n) = floor(10^(n+1)/81).
 * https://oeis.org/A057932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(10^(n+1)/81)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = 0;                /* a(0) = 0 */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t d = i % 9;          /* pattern 0,1,…,8 */
        if (d == 8) d = 9;        /* 8%9 → 9 to get the digit 9 in the cycle */
        cur = cur * 10 + (seq_t)d;/* append the next digit */
    }
    return cur;
}

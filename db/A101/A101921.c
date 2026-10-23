/*
 * A101921  a(2n) = a(n) + 2n - 1, a(2n+1) = 4n.
 * https://oeis.org/A101921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n) = a(n) + 2n - 1, a(2n+1) = 4n, a(1) = 0
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    idx_t cur = n;
    while (cur > 1) {
        if ((cur & 1) == 0) {          /* cur is even: a(cur) = a(cur/2) + cur - 1 */
            ans += (seq_t)cur - 1;
            cur >>= 1;
        } else {                       /* cur is odd (>1): a(cur) = 2*(cur - 1) */
            ans += (seq_t)2 * ((seq_t)cur - 1);
            return ans;
        }
    }
    return ans;                         /* cur == 1 -> a(1) = 0 */
}

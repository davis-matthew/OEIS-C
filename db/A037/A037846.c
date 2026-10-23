/*
 * A037846  a(n)=Sum{d(i-1)-d(i): d(i)<d(i-1), i=1,...,m}, where Sum{d(i)*5^i:
 *          i=0,1,...,m} is the base 5 representation of n.
 * https://oeis.org/A037846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=1}^{m} max(0, d_{i-1} - d_i) where n = Σ_{i=0}^{m}
 *            d_i * 5^i and each d_i ∈ {0,…,4}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n == 0) 
        return 0;                /* safety for edge cases */
    idx_t prev = n % 5;          /* least significant digit */
    n /= 5;
    while (n > 0) {
        idx_t cur = n % 5;       /* next higher digit */
        if (cur < prev) {
            res += (seq_t)(prev - cur);
        }
        prev = cur;
        n /= 5;
    }
    return res;
}

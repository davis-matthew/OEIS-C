/*
 * A037859  Sum{d(i)-d(i-1): d(i)>d(i-1), i=1,...,m}, where Sum{d(i)*9^i:
 *          i=0,1,...,m} is base 9 representation of n.
 * https://oeis.org/A037859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{i=1}^{m} [ d(i) > d(i-1) ? d(i)-d(i-1) : 0 ] * where n
 *            = Σ_{i=0}^{m} d(i)·9^i is the base‑9 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* protect against out‑of‑range 0 */
    seq_t sum = 0;
    idx_t prev = n % 9;            /* d(0) */
    n /= 9;
    while (n > 0) {
        idx_t cur = n % 9;         /* d(i) */
        if (cur > prev) {
            sum += (seq_t)(cur - prev);
        }
        prev = cur;
        n /= 9;
    }
    return sum;
}

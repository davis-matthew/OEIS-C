/*
 * A037858  Sum{d(i)-d(i-1): d(i)>d(i-1), i=1,...,m}, where Sum{d(i)*8^i:
 *          i=0,1,...,m} is base 8 representation of n.
 * https://oeis.org/A037858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=1}^{m} (d_i - d_{i-1}) if d_i > d_{i-1}, where
 *            d_0,d_1,…,d_m are the base‑8 digits of n (least significant
 *            first)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return 0;           /* for safety; offset starts at 1 */
    idx_t prev = n % 8;
    n /= 8;
    while (n > 0) {
        idx_t curr = n % 8;
        if (curr > prev) {
            sum += (seq_t)(curr - prev);
        }
        prev = curr;
        n /= 8;
    }
    return sum;
}

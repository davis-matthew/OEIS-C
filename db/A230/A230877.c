/*
 * A230877  If n = Sum_{i=0..m} c(i)*2^i, c(i) = 0 or 1, then a(n) =
 *          Sum_{i=0..m} (m+1-i)*c(i).
 * https://oeis.org/A230877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = Σ_{i=0..m} (m+1-i)·c_i, where n = Σ_{i=0..m} c_i·2^i,
 *            c_i∈{0,1}
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */

    /* Determine the highest set bit (m = floor(log2(n))) */
    idx_t m = 0;
    idx_t tmp = n;
    while (tmp >>= 1) ++m;          /* tmp becomes 1 at loop end, m is position of that bit */

    /* Sum the weighted contributions of each set bit */
    seq_t result = 0;
    for (idx_t i = 0; i <= m; ++i) {
        if (n & ((idx_t)1 << i)) {
            result += (seq_t)(m + 1 - i);
        }
    }
    return result;
}

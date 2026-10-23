/*
 * A037808  Number of i such that d(i) < d(i-1), where Sum_{i=0..m} d(i)*10^i
 *          is the base-10 representation of n.
 * https://oeis.org/A037808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = #{ i>0 : d_i < d_{i-1} }, where n = Σ d_i·10^i is the
 *            base‑10 expansion of n (digits counted from least significant)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) return 0;          /* for completeness */
    idx_t prev = n % 10;
    n /= 10;
    while (n > 0)
    {
        idx_t cur = n % 10;
        if (cur < prev) ++count;
        prev = cur;
        n /= 10;
    }
    return count;
}

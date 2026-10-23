/*
 * A037810  Number of i such that d(i) <= d(i-1), where Sum_{i=0..m} d(i)*3^i
 *          is the base-3 representation of n.
 * https://oeis.org/A037810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of indices i≥1 such that d_i ≤ d_{i-1} in the
 *            base‑3 digit expansion of n (d_0 least significant)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) return count;          /* offset starts at 1, but handle 0 safely */

    seq_t prev = n % 3;                 /* d_0 */
    n /= 3;
    while (n > 0) {
        seq_t cur = n % 3;              /* d_i for i ≥ 1 */
        if (cur <= prev) count++;
        prev = cur;
        n /= 3;
    }
    return count;
}

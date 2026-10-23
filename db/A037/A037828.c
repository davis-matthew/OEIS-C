/*
 * A037828  Number of i such that d(i) >= d(i-1), where Sum_{i=0..m} d(i)*5^i
 *          is the base-5 representation of n.
 * https://oeis.org/A037828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of indices i ≥ 1 such that d(i) ≥ d(i − 1), where
 *            n = Σ_{i=0..m} d(i)·5^i is the base‑5 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n ≥ 1 is assumed. */
    seq_t count = 0;
    if (n == 0) {
        return count;   /* safety for n = 0, though offset is 1 */
    }

    int prev = (int)(n % 5);   /* least significant digit */
    n /= 5;

    while (n > 0) {
        int cur = (int)(n % 5);
        if (cur >= prev) {
            ++count;
        }
        prev = cur;
        n /= 5;
    }

    return count;
}

/*
 * A037906  Number of i such that |d(i) - d(i-1)| = 1, where Sum_{i=0..m}
 *          d(i)*3^i is the base-3 representation of n.
 * https://oeis.org/A037906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of i such that |d(i) - d(i-1)| = 1 in the base‑3
 *            representation of n, where n = Σ d(i)·3^i
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset is 1, but we guard against n == 0 for safety */
    if (n == 0) {
        return 0;
    }

    /* least significant digit */
    idx_t prev = n % 3;
    n /= 3;

    seq_t count = 0;

    while (n > 0) {
        idx_t cur = n % 3;
        if ((cur == prev + 1) || (cur + 1 == prev)) {
            ++count;
        }
        prev = cur;
        n /= 3;
    }

    return count;
}

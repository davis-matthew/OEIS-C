/*
 * A037910  Number of i such that |d(i) - d(i-1)| = 1, where Sum_{i=0..m}
 *          d(i)*7^i is the base-7 representation of n.
 * https://oeis.org/A037910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of i such that |d(i)-d(i-1)| = 1, where the base–7
 *            digits d(i) of n are defined by n = Σ_{i=0..m} d(i)*7^i
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* safety; sequence starts at n=1 */

    idx_t prev = n % 7;          /* least significant digit */
    n /= 7;
    seq_t count = 0;

    while (n > 0) {
        idx_t curr = n % 7;      /* next digit */
        if ((prev > curr ? prev - curr : curr - prev) == 1)
            count++;             /* adjacent digits differ by 1 */
        prev = curr;
        n /= 7;
    }

    return count;
}

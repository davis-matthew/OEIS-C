/*
 * A116543  Number of terms in greedy representation of n in terms of the
 *          Lucas numbers.
 * https://oeis.org/A116543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 + a(n - L(n)) (greedy Lucas representation), * where
 *            L(n) is the largest Lucas number ≤ n, and a(0) = 0. * This
 *            implementation runs in O(a(n) · log n) time and uses O(1) *
 *            additional memory, so it easily satisfies the 2‑second limit for
 *            * n ≤ 10000 on all supported integer arities
 * Range:     u8 n <= 186, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t count = 0;
    while (n > 0) {
        /* Find the largest Lucas number ≤ n. */
        seq_t largest = 1;           /* Lucas(1) = 1 */
        if (n >= 2)
            largest = 2;             /* Lucas(0) = 2 */
        /* Generate Lucas numbers starting from L0=2, L1=1. */
        seq_t prev2 = 2;             /* L0 */
        seq_t prev1 = 1;             /* L1 */
        while (1) {
            seq_t next = prev2 + prev1;
            if (next > n)
                break;
            largest = next;
            prev2 = prev1;
            prev1 = next;
        }

        n -= (idx_t)largest;
        count++;
    }
    return count;
}

/*
 * A091627  Number of ordered integer pairs (b,c) with 1 <= b,c <= n such that
 *          both roots of x^2+bx+c=0 are integers.
 * https://oeis.org/A091627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{i=1}^{n} \max(0,\min(n-i,\;n/i)-i+1)
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of ordered integer pairs (b,c) with
 * 1 ≤ b,c ≤ n such that both roots of x² + b·x + c = 0 are integers.
 * This is equivalent to counting unordered pairs (i,j) of positive integers
 * with i + j ≤ n and i·j ≤ n.  The following O(n) algorithm
 * implements the arithmetic that follows from that equivalence. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 1; i <= n; ++i) {
        /* j must satisfy  i ≤ j ≤ n – i   and   i·j ≤ n   */
        idx_t max_j = n - i;          /* from i + j ≤ n */
        idx_t div = n / i;            /* from i·j ≤ n  →   j ≤ n/i */
        if (div < max_j) max_j = div; /* take the tighter bound   */
        if (max_j >= i) {
            /* add the number of j values from i to max_j inclusive */
            total += (seq_t)(max_j - i + 1);
        }
    }
    return total;
}

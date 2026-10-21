/*
 * A211536  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w=4x-5y.
 * https://oeis.org/A211536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{y=1..n} max(0, min(n, ⌊(n+5y)/4⌋) - ⌈(5y+1)/4⌉ + 1)
 *            which counts the number of integer triples (w,x,y) with 1 ≤
 *            w,x,y ≤ n and w = 4x – 5y. The function returns this value for
 *            any n ≥ 0
 * Range:     u8 n <= 38, u16 n <= 612, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t n_seq = (seq_t)n;

    for (idx_t y = 1; y <= n; ++y)
    {
        seq_t y_seq = (seq_t)y;
        /* ceil((5*y + 1)/4) = (5*y + 4)/4 for positive integers */
        seq_t low  = (5 * y_seq + 4) / 4;
        /* floor((n + 5*y)/4) */
        seq_t high = (n_seq + 5 * y_seq) / 4;

        if (high > n_seq) high = n_seq;          /* enforce x <= n */
        if (low > high) continue;                /* no admissible x */

        total += high - low + 1;
    }

    return total;
}

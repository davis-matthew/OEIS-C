/*
 * A176615  Number of edges in the graph on n vertices, labeled 1 to n, where
 *          two vertices are joined just if their labels sum to a perfect
 *          square.
 * https://oeis.org/A176615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = number of pairs i < j ≤ n with i + j a perfect square
 * Range:     u8 n <= 96, u16 n <= 3834, u32 n <= 9948, u64 n <= 9948,
 *            u128 n <= 9948, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t cnt = 0;

    /* compute the largest s such that s^2 ≤ 2*n */
    idx_t max_s = 0;
    while ((max_s + 1) * (max_s + 1) <= 2 * n) {
        ++max_s;
    }

    for (idx_t s = 2; s <= max_s; ++s) {
        idx_t s2 = s * s;

        /* i must satisfy 1 ≤ i < s^2/2 and i ≥ s^2 - n */
        idx_t lower = (s2 > n) ? (s2 - n) : 1;
        idx_t upper = (s2 - 1) / 2;
        if (upper > n) upper = n;

        if (upper >= lower) {
            cnt += (seq_t)(upper - lower + 1);
        }
    }

    return cnt;
}

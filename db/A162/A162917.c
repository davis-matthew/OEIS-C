/*
 * A162917  Numbers n which are not in A161983.
 * https://oeis.org/A162917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = m^2 + (n - 1), where m is the smallest integer satisfying
 *            m(m + 1) ≥ n
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t
find_block(idx_t n)
{
    /* Find minimal m with m*(m+1) >= n.  Use __int128 to avoid overflow. */
    idx_t m = 0;
    while ((unsigned __int128)m * (m + 1) < n)
        ++m;
    return m;
}

seq_t
a(idx_t n)
{
    /* n is 1‑based; the sequence is defined only for n ≥ 1. */
    idx_t m = find_block(n);
    return (seq_t)m * (seq_t)m + (seq_t)(n - 1);
}

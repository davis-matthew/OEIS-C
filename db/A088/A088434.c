/*
 * A088434  Number of ways to write n as n = u*v*w with 1 <= u < v < w.
 * https://oeis.org/A088434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts triples (u,v,w) with 1 ≤ u < v < w and u·v·w = n
 * Range:     u8 n <= 1459, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t N = (seq_t)n;
    seq_t count = 0;

    for (idx_t u = 1; (seq_t)u * (seq_t)u * (seq_t)u <= N; ++u)
    {
        if (N % (seq_t)u != 0) continue;
        seq_t m = N / (seq_t)u;                /* now need v·w = m, u < v < w */

        for (idx_t v = u + 1; (seq_t)v * (seq_t)v <= m; ++v)
        {
            if (m % (seq_t)v != 0) continue;
            seq_t w = m / (seq_t)v;
            if (v < w) ++count;                /* ensures u < v < w */
        }
    }
    return count;
}

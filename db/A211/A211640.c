/*
 * A211640  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2+x^2+y^2>n.
 * https://oeis.org/A211640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 - #{(w,x,y)∈[1..⌊√n⌋]^3 | w^2 + x^2 + y^2 ≤ n}
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute ⌊√n⌋ */
    idx_t m = 0;
    while ((m + 1) * (m + 1) <= n)
        ++m;

    seq_t total_count = 0;
    seq_t N = n; /* cast once for comparisons */

    /* Count triples with sum of squares ≤ n */
    for (idx_t w = 1; w <= m; ++w) {
        seq_t wsq = (seq_t)w * w;
        for (idx_t x = 1; x <= m; ++x) {
            seq_t xsq = (seq_t)x * x;
            seq_t wx  = wsq + xsq;
            for (idx_t y = 1; y <= m; ++y) {
                seq_t sum = wx + (seq_t)y * y;
                if (sum <= N)
                    ++total_count;
            }
        }
    }

    /* a(n) = n^3 - total_count */
    seq_t n3 = (seq_t)n * n * n;
    return n3 - total_count;
}

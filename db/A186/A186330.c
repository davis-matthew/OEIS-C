/*
 * A186330  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the pentagonal numbers and
 *          the hexagonal numbers. Complement of A186331.
 * https://oeis.org/A186330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + # {j ≥ 1 : H(j) < P(n)} + (1 if P(n) equals a
 *            hexagonal number, else 0), where P(n) = n(3n‑1)/2 and H(j) =
 *            j(2j‑1)
 * Range:     u8 n <= 12, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined but safe fallback */

    /* Compute pentagonal number P(n).  Use seq_t arithmetic to avoid overflow. */
    seq_t nn = (seq_t)n;
    seq_t P  = nn * ((seq_t)3 * nn - 1) / 2;   /* P(n) = n(3n-1)/2 */

    /* Count hexagonal numbers strictly less than P(n) using binary search.
       The answer is at most n, so we can use [0,n] as the search interval. */
    idx_t lo = 0;
    idx_t hi = n;                     /* hi is guaranteed > the desired count */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        seq_t m   = (seq_t)mid;
        seq_t Hmid = m * ((seq_t)2 * m - 1);   /* H(mid) = mid(2*mid-1) */
        if (Hmid < P) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    idx_t hex_lt = lo;                     /* number of hexagonal numbers < P */

    /* Base rank: counts of pentagonal numbers (n-1) plus the current pentagonal plus
       the hexagonal numbers smaller than it. */
    seq_t rank = (seq_t)n + (seq_t)hex_lt;  /* n + hex_lt */

    /* Add one if P(n) equals a hexagonal number (this occurs for n=1 only). */
    seq_t next_j = (seq_t)(hex_lt + 1);
    seq_t Hnext = next_j * ((seq_t)2 * next_j - 1);
    if (Hnext == P) {
        rank += 1;
    }

    return rank;
}

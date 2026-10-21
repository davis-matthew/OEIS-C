/*
 * A053526  Number of bipartite graphs with 3 edges on nodes {1..n}.
 * https://oeis.org/A053526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3)*(n-2)*(n-1)*n*(n^2 + 3*n + 4)/48
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 76, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) {
        return 0;
    }

    seq_t sn = n;                                 /* convert n to seq_t */
    seq_t p1 = sn - (seq_t)3;
    seq_t p2 = sn - (seq_t)2;
    seq_t p3 = sn - (seq_t)1;

    seq_t prod = p1 * p2;
    prod = prod * p3;
    prod = prod * sn;                              /* (n-3)*(n-2)*(n-1)*n */

    seq_t poly = sn * sn + (seq_t)3 * sn + (seq_t)4; /* n^2 + 3*n + 4 */

    prod = prod * poly;                            /* multiply by (n^2 + 3*n + 4) */

    prod = prod / (seq_t)48;                       /* divide by 48 (exact division) */

    return prod;
}

/*
 * A060534  Number of homeomorphically irreducible multigraphs (or
 *          series-reduced multigraphs or multigraphs without nodes of degree
 *          2) on 4 labeled nodes.
 * https://oeis.org/A060534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 60 + 48*(1+n) - 12*(1+n)*(2+n) +
 *            (1+n)(2+n)(3+n)(4+n)(5+n)/120 for n>6
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 185, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t first[7] = {1,6,3,10,48,84,182};
    if (n <= 6)
        return first[n];

    idx_t N = n;

    seq_t n1 = (seq_t)(N + 1);
    seq_t term2 = 48 * n1;
    seq_t prod = 12 * n1 * (N + 2);

    seq_t binom = 1;
    for (idx_t i = 1; i <= 5; ++i) {
        binom = binom * (N + i);
        binom = binom / i;
    }

    seq_t res = binom;
    res += 60;
    res += term2;
    res -= prod;
    return res;
}

/*
 * A098606  Sum of all matrix elements M(i,j) = (i^2)/(i+j).
 * https://oeis.org/A098606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)! * Sum_{i=1..n} Sum_{j=1..n} i^2/(i+j)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* compute (2n)! */
    seq_t fact = 1;
    for (idx_t k = 2; k <= 2 * n; ++k) {
        fact *= (seq_t)k;
    }

    seq_t result = 0;

    for (idx_t u = 2; u <= 2 * n; ++u) {
        /* determine a = max(1, u-n) and b = min(n, u-1) */
        idx_t a = (u > n) ? (u - n) : 1;
        idx_t b = (u - 1 <= n) ? (u - 1) : n;

        /* sum of squares from a to b: (b(b+1)(2b+1) - (a-1)a(2(a-1)+1)) / 6 */
        seq_t A = (seq_t)a;
        seq_t B = (seq_t)b;
        seq_t B1 = B * (B + 1);
        seq_t term1 = B1 * (2 * B + 1);

        seq_t A1 = (a > 0) ? (seq_t)(a - 1) : 0;
        seq_t term2 = A1 * A * (2 * A1 + 1);

        seq_t sumsq = (term1 - term2) / 6;

        /* (2n)! / u is integer */
        seq_t fact_div = fact / (seq_t)u;

        result += fact_div * sumsq;
    }

    return result;
}

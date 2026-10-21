/*
 * A094385  Triangle read by rows: T(n, k) = binomial(2*n,
 *          k-1)*binomial(2*n-k-1, n-k)/n for n, k >= 1, and T(n, 0) = 0^n.
 * https://oeis.org/A094385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,c) where r = ⌊(√(8n+1)−1)/2⌋, c = n−r(r+1)/2. T(n,k)
 *            = binomial(2n,k−1)·binomial(2n−k−1,n−k)/n (n,k≥1), T(n,0)=0ⁿ.
 *            a(0)=1. The sequence is the triangle read by rows
 * Range:     u8 n <= 15, u16 n <= 43, u32 n <= 125, u64 n <= 361,
 *            u128 n <= 1195, big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* find row r and the start index of that row */
    idx_t r = 0;
    idx_t start = 0;
    while (n >= start + (r + 1)) {
        start += r + 1;
        r++;
    }
    /* column index c */
    idx_t c = n - start;

    if (r == 0)          /* the only element of row 0 */
        return (seq_t)1;

    if (c == 0)          /* entries T(r,0) for r≥1 are 0 */
        return (seq_t)0;

    /* compute the two binomial factors */
    seq_t b1 = binom(2 * r, c - 1);
    seq_t b2 = binom(2 * r - c - 1, r - c);

    seq_t rseq = (seq_t)r;

    /* reduce by gcd to avoid overflow in the product division */
    seq_t g  = gcd(b1, rseq);
    seq_t r1 = rseq / g;
    seq_t b1d = b1 / g;

    g  = gcd(b2, r1);
    seq_t r2 = r1 / g;
    seq_t b2d = b2 / g;

    seq_t result = b1d * b2d;
    if (r2 > (seq_t)1)
        result = result / r2;

    return result;
}

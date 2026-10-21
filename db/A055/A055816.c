/*
 * A055816  a(n) = T(2*n+4,n), array T as in A055807.
 * https://oeis.org/A055816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{s=1..n+4} binomial(n+4,s) * binomial(s+n-2,n-1) (n ≥
 *            1), a(0) = 1. This computes the OEIS sequence A055816
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t r = n + 4;          /* upper limits for the outer binomial coefficient */
    idx_t k = n - 1;          /* the second binomial has parameters (s+k-1, k) */

    seq_t c1 = (seq_t)1;      /* C(r,0) initially */
    seq_t c2 = (seq_t)1;      /* C(k, k) gives the value for s = 1 */
    seq_t sum = (seq_t)0;

    for (idx_t s = 1; s <= r; ++s) {
        /* Update c1 to C(r, s) */
        c1 = c1 * (seq_t)(r - s + 1) / (seq_t)s;

        /* Add the product of the two binomials for this s */
        sum += c1 * c2;

        /* Prepare c2 for the next iteration (s + 1) :
           c2_{s+1} = c2_s * (s + k) / s  , valid when s < r               */
        if (s < r)
            c2 = c2 * (seq_t)(s + k) / (seq_t)s;
    }

    return sum;
}

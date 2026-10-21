/*
 * A225997  Number of pairs of orthogonal (-1,6) vectors of length 7*n.
 * https://oeis.org/A225997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 * Σ_{b=0}^n C(7n, 6(n-b)) * C(7n-6(n-b), b) *
 *            2^{n+5b}
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of unordered pairs of orthogonal (-1,6)-vectors of length 7·n. */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* L = 7n is the length of the vectors. */
    idx_t L = 7 * n;
    seq_t sum = 0;                 /* accumulate ordered pairs */
    for (idx_t b = 0; b <= n; ++b)
    {
        /* counts for the three block types: a, b, s */
        idx_t a_cnt = 6 * (n - b);
        idx_t s_cnt = n + 5 * b;

        /* multinomial factor C(L,a) * C(L-a,b) */
        seq_t term = binomial(L, a_cnt);
        term *= binomial(L - a_cnt, b);

        /* multiply by 2^{s} */
        term *= pow2(s_cnt);

        sum += term;
    }

    /* unordered pairs → divide by 2. */
    return sum / 2;
}

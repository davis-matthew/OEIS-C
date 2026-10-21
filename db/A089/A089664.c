/*
 * A089664  a(n) = S2(n,1), where S2(n, t) = Sum_{k=0..n} k^t *(Sum_{j=0..k}
 *          binomial(n,j))^2.
 * https://oeis.org/A089664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/8)*(n*(3*n+5)*4^n - 2*n*(n-1)*binomial(2*n, n))
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* If seq_t is 8‑bytes or less, use unsigned __int128 for intermediate
       calculations to avoid overflow when 4^n is large. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 n128 = (unsigned __int128)n;

        /* compute 4^n in 128‑bit */
        unsigned __int128 pow4 = 1;
        for (idx_t i = 0; i < n; ++i)
            pow4 *= 4;

        /* first term: n*(3*n+5)*4^n */
        unsigned __int128 term1 = n128 * (3*n128 + 5);
        term1 *= pow4;

        /* second term: 2*n*(n-1)*binomial(2*n, n) */
        unsigned __int128 bin = (unsigned __int128)binomial(2*n, n);
        unsigned __int128 term2 = 2*n128*(n128-1);
        term2 *= bin;

        /* numerator and divide by 8 */
        unsigned __int128 num = term1 - term2;
        return (seq_t)(num >> 3);
    } else {
        /* seq_t is an arbitrary‑precision type; use its native multiplication. */
        seq_t n_seq = (seq_t)n;
        seq_t pow4  = big_pow((seq_t)4, (seq_t)n);

        seq_t term1 = n_seq * (3*n_seq + 5);
        term1       = term1 * pow4;

        seq_t bin   = binomial(2*n, n);
        seq_t term2 = 2*n_seq * (n_seq - 1);
        term2       = term2 * bin;

        return (term1 - term2) / (seq_t)8;
    }
}

/*
 * A178113  Transform of C(n+1,floor((n+1)/2)) by A178112.
 * https://oeis.org/A178113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 (n = 0 terminates) For n even: a(2m) = Σ_{k=0..m}
 *            C(m,k) * C(k,⌊k/2⌋) For n odd : a(2m+1) = 2*a(2m) where C(n,k)
 *            is binomial(n,k)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(idx_t n) returns the n-th term of OEIS A178113
 *  (indexing starts at 0).  */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* m = floor(n/2)  */
    idx_t m = n / 2;

    /* compute the sum e(m) = Σ_{k=0..m} C(m,k) * C(k,⌊k/2⌋)  */
    seq_t binom = 1;  /* C(m,0)   */
    seq_t c     = 1;  /* C(0,0)   */
    seq_t sum   = 0;

    idx_t k;
    for (k = 0; k <= m; ++k)
    {
        /* add current term to the sum */
        sum += binom * c;

        if (k == m)
            break;  /* finished all terms */

        /* update binom for next k     : binom_{k+1} = binom_k * (m-k) / (k+1) */
        seq_t num = binom * (seq_t)(m - k);
        binom = num / (seq_t)(k + 1);

        /* update c for next k (C(k,⌊k/2⌋)) */
        idx_t nk = k + 1;
        if (nk % 2 == 0)        /* even: C(2t,t) = 2 * C(2t-1, t-1) */
        {
            c = c * 2;
        }
        else                    /* odd:  C(2t+1,t) = C(2t,t) * (2t+1)/(t+1) */
        {
            seq_t denom = (seq_t)(nk / 2 + 1);
            c = (c * (seq_t)nk) / denom;
        }
    }

    /* If the requested index is odd, double the even value */
    if (n & 1)
        sum = sum * (seq_t)2;

    return sum;
}

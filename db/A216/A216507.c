/*
 * A216507  E.g.f. exp( x^2 * exp(x) ).
 * https://oeis.org/A216507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..floor(n/2)} C(n,2k) * ((2k)!/k!) * k^(n-2k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) -> seq_t */
#include "../utils/pow_uint.h"   /* pow_uint(uint64_t base, idx_t exp) -> seq_t */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t maxk = n / 2;
    seq_t prod   = 1;                /* (2k)!/k! term, starts with k=0 ⇒ 1 */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* binomial coefficient C(n,2k) */
        seq_t bin = binomial(n, 2 * k);

        /* k^(n-2k) with special handling for k==0 to make 0^0 == 1 */
        seq_t pw;
        if (k == 0)
            pw = (n == 0 ? (seq_t)1 : (seq_t)0);
        else
            pw = pow_uint((uint64_t)k, n - 2 * k);

        /* accumulate contribution for this k */
        seq_t term = bin * prod * pw;
        result += term;

        /* update prod for next k (if any)
           prod_next = prod * (2k+1)*(2k+2) / (k+1)          */
        if (k < maxk)
        {
            seq_t f1 = (seq_t)(2 * k + 1);
            seq_t f2 = (seq_t)(2 * k + 2);
            prod = prod * f1;
            prod = prod * f2;
            prod = prod / (seq_t)(k + 1);
        }
    }

    return result;
}

/*
 * A050984  de Bruijn's S(5,n) = Sum_{k = 0..2*n} (-1)^(n+k)*binomial(2*n,
 *          k)^5.
 * https://oeis.org/A050984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..2*n} (-1)^(n+k) * binomial(2*n, k)^5
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the provided binomial() and pow_seq() helpers. */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t pos = 0;     /* sum of terms with positive sign   */
    seq_t neg = 0;     /* sum of terms with negative sign   */
    idx_t twon = 2 * n;                                     /* 2*n fits in idx_t  */

    for (idx_t k = 0; k <= twon; ++k)
    {
        seq_t c  = binomial(twon, k);     /* C(2*n, k)  */
        seq_t c5 = pow_seq(c, 5);         /* C(2*n, k)^5 */

        /* sign = (-1)^(n+k)  => even -> +, odd -> -  */
        if ( ((n + k) & 1ULL) == 0 )
            pos += c5;
        else
            neg += c5;
    }

    /* For all n ≥ 0 the final result is positive, so pos ≥ neg  */
    return pos - neg;
}

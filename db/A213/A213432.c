/*
 * A213432  a(n) = 2^(n-3)*binomial(n,4).
 * https://oeis.org/A213432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-3) * binomial(n,4) for n ≥ 0, with
 *            a(0)=a(1)=a(2)=a(3)=0
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 22, u64 n <= 49, u128 n <= 108,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;

    /* binomial(n,4) and pow2(n-3) are returned as seq_t */
    seq_t bin = binomial(n, 4);
    seq_t p2  = pow2(n - 3);

    return p2 * bin;
}

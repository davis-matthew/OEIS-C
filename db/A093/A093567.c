/*
 * A093567  Binomial (Binomial (n,2), 3) - Binomial (Binomial (n,3), 2).
 * https://oeis.org/A093567
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(binomial(n,2),3) - binomial(binomial(n,3),2)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* Compute the two binomial terms in the definition */
    seq_t c2 = binomial(n, 2);   /* C(n,2)  */
    seq_t c3 = binomial(n, 3);   /* C(n,3)  */

    /* Now compute C(c2,3) and C(c3,2).  
       For the intended range (n up to ~10000) c2 and c3 fit in 64 bits,
       so casting to idx_t is safe. */
    seq_t t1 = binomial((idx_t)c2, 3);
    seq_t t2 = binomial((idx_t)c3, 2);

    /* Final result: difference of the two binomials */
    return t1 - t2;
}

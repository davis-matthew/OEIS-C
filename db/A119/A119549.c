/*
 * A119549  a(n) = binomial(Catalan(n), 4).
 * https://oeis.org/A119549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(catalan(n), 4)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Compute the Catalan number C = Catalan(n). */
    seq_t C = catalan(n);

    /* If C < 4 then binomial(C,4) = 0. */
    if (C < (seq_t)4)
        return (seq_t)0;

    /* Compute binomial(C,4) multiplicatively:
       res = 1;
       for i = 1 .. 4:
           res = res * (C - (i-1));
           res = res / i;
    */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= (idx_t)4; ++i)
    {
        res = res * (C - (seq_t)(i - (idx_t)1));
        res = res / (seq_t)i;
    }
    return res;
}

/*
 * A069723  a(n) = 2^(n-1)*binomial(2*n-3, n-1).
 * https://oeis.org/A069723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n-1)*binomial(2*n-3,n-1) (offset 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   binomial, pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq_t.h"

seq_t a(idx_t n)
{
    /* n = 1 is a special case because 2*n-3 = -1 is outside the
       domain of the binomial helper. */
    if (n == 1)
        return (seq_t)1;

    /* 2^(n-1) – use the generic power helper to avoid overflow. */
    seq_t pow2 = pow_seq_t((seq_t)2, n - 1);

    /* binomial(2n-3, n-1) – both arguments are non‑negative for n>=2. */
    seq_t comb = binomial(2 * n - 3, n - 1);

    /* Final product.  The multiplication works for any seq_t that
       supports integer arithmetic; for fixed‑width types it
       overflows if the result does not fit (which is acceptable
       because terms that do not fit are not required to be correct). */
    return pow2 * comb;
}

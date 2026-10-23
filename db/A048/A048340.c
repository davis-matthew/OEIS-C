/*
 * A048340  a(n) in base 16 is a repdigit.
 * https://oeis.org/A048340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n - 15*floor((n-1)/15)) * (16^floor((n+14)/15) - 1) / 15
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 120, u64 n <= 240,
 *            u128 n <= 480, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = t * (1 + 16 + 16^2 + ... + 16^(L-1)) */
/* where t = ((n-1) % 15) + 1 and L = (n + 14) / 15. */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint*_t types */

seq_t a(idx_t n)
{
    /* Handle the offset term. */
    if (n == 0)
        return (seq_t)0;

    /* Determine the block length L and the digit rank t. */
    idx_t L = (n + 14) / 15;          /* ceil(n/15)  */
    idx_t t = (n - 1) % 15 + 1;       /* 1..15 */

    /* Compute S = 1 + 16 + 16^2 + ... + 16^(L-1)  */
    /* The partial sum fits in seq_t for all n that produce a value which fits in seq_t. */
    seq_t sum = 1;
    seq_t term = 1;
    for (idx_t i = 1; i < L; ++i) {
        term *= (seq_t)16;
        sum += term;
    }

    /* Result is t * sum.  For the supported seq_t sizes this product
       never overflows when the final value fits in seq_t.  */
    seq_t result = (seq_t)((seq_t)t * sum);

    return result;
}

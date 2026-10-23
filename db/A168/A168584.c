/*
 * A168584  Number of ways of partitioning the multiset {1,1,2,3,...,n-1} into
 *          exactly four nonempty parts.
 * https://oeis.org/A168584
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*4^(n-3) - 3^(n-1) + 3*2^(n-2) - 2)/3, for n ≥ 4
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

/* Computes the n‑th term of OEIS A168584.  The implementation uses the
   closed‑form expression and the generic integer exponentiation routine
   pow_int from the build utilities.  For n < 4 the function returns 0
   (behavior below the offset is irrelevant). */
seq_t a(idx_t n)
{
    if (n < 4)
        return 0;   /* undefined below the offset */

    /* Compute the required powers using pow_int.  The exponent is
       an idx_t (unsigned 64‑bit), which why we pass the value directly. */
    seq_t p4 = pow_int((seq_t)4, n - 3);      /* 4^(n-3)   */
    seq_t p3 = pow_int((seq_t)3, n - 1);      /* 3^(n-1)   */
    seq_t p2 = pow_int((seq_t)2, n - 2);      /* 2^(n-2)   */

    /* Apply the closed‑form formula. */
    seq_t num = 5 * p4 - p3 + 3 * p2 - 2;
    return num / 3;
}

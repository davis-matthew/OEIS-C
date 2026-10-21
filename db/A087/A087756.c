/*
 * A087756  a(n) = A087745(n+1).
 * https://oeis.org/A087756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   a(n) = product of Fermat numbers 2^(2^i)+1 for each bit i set in
 *            floor((n+1)/2). This equals A001317(floor((n+1)/2)) and
 *            implements A087756, which lists each term of A001317 twice
 *            except for the first
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* Compute index j = floor((n+1)/2) from OEIS definition a(n)=A001317(j). */
    idx_t j = (n + 1) >> 1;

    /* Result starts at 1. */
    seq_t result = (seq_t)1;

    /* For each bit i set in j, multiply by the i‑th Fermat number
       F_i = 2^(2^i) + 1.  The exponent 2^i is computed as 1ULL << i. */
    for (idx_t i = 0; (j >> i) > 0; ++i)
    {
        if (j & ((idx_t)1 << i))
        {
            idx_t exp = ((idx_t)1 << i);     /* 2^i */
            seq_t f = pow2(exp);             /* 2^(2^i) */
            f += (seq_t)1;                   /* 2^(2^i) + 1 */
            result *= f;                     /* accumulate product */
        }
    }
    return result;
}

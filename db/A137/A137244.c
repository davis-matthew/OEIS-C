/*
 * A137244  a(n) = lcm_{k=0..n} (k! + 1).
 * https://oeis.org/A137244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm_{k=0..n} (k! + 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   _gcd_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_gcd_local.h"

seq_t a(idx_t n)
{
    seq_t fact = (seq_t)1;          /* factorial accumulator */
    seq_t lcm_val = (seq_t)0;        /* current lcm (will be set at k==0) */

    for (idx_t k = (idx_t)0; k <= n; ++k)
    {
        if (k > (idx_t)0)
            fact = fact * (seq_t)k;   /* compute k! incrementally */

        seq_t term = fact + (seq_t)1;  /* k! + 1 */

        if (k == (idx_t)0)
        {
            lcm_val = term;
            continue;
        }

        seq_t g = _gcd_local(lcm_val, term);
        lcm_val = (lcm_val / g) * term;
    }

    return lcm_val;
}

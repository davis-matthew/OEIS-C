/*
 * A178511  a(n) = (1/119)*(100^n - (-19)^n).
 * https://oeis.org/A178511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   mul_by_const
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A178511
   Formula: a(n) = (100^n - (-19)^n) / 119
   (For all n ≥ 1.  a(1)=1, a(2)=81, …)
   The implementation uses the recurrence
       a(1) = 1
       a(k+1) = a(k)*100 + (-19)^k
   and maintains the alternating power of 19.  For integer
   seq_t types the product a(k)*100 is calculated with an
   unsigned __int128 intermediate to avoid overflow, while for
   arbitrary‑precision seq_t the built‑in multiplication operator
   is used.  This works both in a C (integer seq_t) and in a
   C++ (arbitrary‑precision seq_t) build. */

#include <stdint.h>
#include "../utils/mul_by_const.h"

seq_t a(idx_t n)
{
    if (n == 0)                     /* below offset; harmless */
        return 0;

    seq_t a_curr = 1;                 /* a(1) */
    seq_t pow19  = 19;                /* 19^1   */
    int sign = -1;                    /* (-1)^1 */

    for (idx_t k = 1; k < n; ++k)
    {
        /* a_next = a_curr * 100 + (-19)^k  */
        seq_t prod = mul_by_const(a_curr, 100ull);
        prod = (sign == 1) ? prod + pow19 : prod - pow19;
        a_curr = prod;

        /* Prepare for next iteration: update power of 19 and sign */
        pow19 = mul_by_const(pow19, 19ull);
        sign = -sign;
    }

    return a_curr;
}

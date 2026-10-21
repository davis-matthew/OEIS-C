/*
 * A210278  a(n) = (5n)!/5^n.
 * https://oeis.org/A210278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A210278
   Formula: a(n) = (5n)! / 5^n
   The function computes the exact value for any n ≥ 0.
   It uses long multiplication by 1..5n and cancels 5‑factors on the fly
   to avoid overflow and keep the intermediate product as small as possible.
   The algorithm runs in O(n) time and uses only integer arithmetic. */

#include <stdint.h>  /* for uint64_t, idx_t may depend on this header */

seq_t a(idx_t n)
{
    /* result = (5n)! / 5^n */
    seq_t result = 1;
    idx_t limit = (idx_t)5 * n;          /* upper bound for k */
    idx_t div_rem = n;                   /* how many 5's still to cancel */

    for (idx_t k = 1; k <= limit; ++k)
    {
        result *= (seq_t)k;              /* multiply by current factor */

        /* cancel a factor of 5 if the current result still has a 5-factor
           and we still have 5's left to divide out. */
        while (div_rem > 0 && (result % 5) == 0)
        {
            result /= 5;
            --div_rem;
        }
    }

    /* For n = 0, the loop is skipped and result stays 1. */
    return result;
}

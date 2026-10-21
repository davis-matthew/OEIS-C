/*
 * A092950  Beginning with n, add the next number, subtract the previous
 *          number and so on until one gets a prime, or 0 if no such prime is
 *          reached in 2n-1 steps: a(n) = n + (n+1) - (n-1) +(n+2) -(n-2)
 *          +(n+3)-(n-3)...+...is the first occurring prime at any step.
 * https://oeis.org/A092950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Implements A092950:
   Starting from n, sequentially add (n+k) and subtract (n–k)
   for k=1,2,….  The first prime encountered (including the
   starting value) is returned; if none occurs within 2 n – 1
   steps, zero is returned. */

seq_t a(idx_t n)
{
    /* Initial value */
    seq_t sum = (seq_t)n;

    /* Check the starting value */
    if (isPrimeGeneric(sum))
        return sum;

    /* Perform at most 2*n–1 additional steps */
    idx_t max_step = 2 * n - 1;
    for (idx_t step = 1; step <= max_step; ++step)
    {
        if (step & 1U)          /* odd step: add (n+k) */
        {
            seq_t k = (seq_t)((step + 1) / 2);
            sum += (seq_t)n + k;
        }
        else                    /* even step: subtract (n–k) */
        {
            seq_t k = (seq_t)(step / 2);
            sum -= (seq_t)n - k;
        }

        if (isPrimeGeneric(sum))
            return sum;
    }

    /* No prime found within allowed steps */
    return (seq_t)0;
}

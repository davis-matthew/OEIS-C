/*
 * A170834  a(n) = 5^(floor(n/2))+5^(floor(n/2)-1)-5^(floor((n-1)/3)).
 * https://oeis.org/A170834
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170834.  Formula: a(n) = 5^(⌊n/2⌋) + 5^(⌊n/2⌋-1) - 5^(⌊(n-1)/3⌋)  */
#include <stdint.h>
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    if (n < 2)                      /* sequence is undefined for n < 2  */
        return 0;

    idx_t half = n / 2;             /* ⌊n/2⌋                            */
    idx_t exp1 = half;              /* 5^(⌊n/2⌋)                        */
    idx_t exp2 = (half > 0) ? half - 1 : 0;  /* 5^(⌊n/2⌋-1)            */
    idx_t exp3 = (n - 1) / 3;        /* ⌊(n-1)/3⌋, 5^⌊(n-1)/3⌋           */

    seq_t term1 = power5(exp1);
    seq_t term2 = power5(exp2);
    seq_t term3 = power5(exp3);

    return term1 + term2 - term3;
}

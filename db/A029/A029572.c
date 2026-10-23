/*
 * A029572  Number of permutations of an n-set containing a 5-cycle.
 * https://oeis.org/A029572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (1 - Sum_{k=0..floor(n/5)} (-1)^k/(k!*5^k))
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only integer arithmetic as required.  It works for any
   integer type seq_t supplied by the build (uint8_t, uint16_t, uint32_t,
   uint64_t, unsigned __int128, or an arbitrary‑precision class).  */
#include <stdint.h>
#include <stddef.h>

/* seq_t and idx_t are provided by the build environment.  They are not
   redefined here. */

seq_t a(idx_t n)
{
    /* For n < 5 no 5‑cycle can appear. */
    if (n < (idx_t)5)
        return (seq_t)0;

    /* m = floor(n / 5)  – the largest k with 5k ≤ n. */
    idx_t m = n / (idx_t)5;

    /* term = n! / 5.  We compute the factorial while applying the
       division by 5 at the point i == 5 to keep intermediate values
       small. */
    seq_t term = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        term *= (seq_t)i;
        if (i == (idx_t)5)
            term /= (seq_t)5;
    }

    /* sum accumulates the alternating series
       a(n) = Σ_{k=1..m} (-1)^{k+1} * n!/(k!*5^k)  */
    seq_t sum = term;   /* k = 1, sign = + */

    /* For k >= 2 we can obtain the next term by dividing the previous
       term by k*5.  The division is exact. */
    for (idx_t k = (idx_t)2; k <= m; ++k)
    {
        idx_t div = k * (idx_t)5;      /* k*5 as a larger integer, then cast */
        term = term / ( (seq_t)div );

        if ( (k & (idx_t)1) == (idx_t)0 )   /* even k → subtract */
            sum -= term;
        else                                 /* odd k  → add      */
            sum += term;
    }

    return sum;
}

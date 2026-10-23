/*
 * A194754  Number of integers k in 1..n such that {k*Pi} < {n*Pi}, where { }
 *          = fractional part.
 * https://oeis.org/A194754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k | 1 ≤ k ≤ n and {k·π} ≤ {n·π}}
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts the integer k≤n whose fractional part after multiplying by π
   is not larger than that of n·π.  The function is O(n), fast for n ≤ 10000. */

#include <math.h>   /* for floorl() */

seq_t a(idx_t n)
{
    /* High‑precision value of π used only for the double calculations below. */
    static const long double PI = 3.141592653589793238462643383279502884L;

    /* Fractional part of n·π (may be calculated with long double). */
    long double npi   = (long double)n * PI;
    long double nfrac = npi - floorl(npi);

    seq_t count = 1;               /* k = n always satisfies the condition   */
    for (idx_t k = 1; k < n; ++k)
    {
        long double kpi   = (long double)k * PI;
        long double kfrac = kpi - floorl(kpi);
        if (kfrac < nfrac)
            ++count;
    }
    return count;
}

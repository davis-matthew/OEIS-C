/*
 * A133298  a(n) = 1 + Sum_{i=1..n} Sum_{j=1..n} Sum_{k=1..n} i^(j+k).
 * https://oeis.org/A133298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{i=1..n} ( Sum_{j=1..n} i^j )^2 = 1 + n^2 +
 *            Sum_{i=2..n} ( i * (i^n - 1)/(i-1) )^2 (OEIS A133298)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    seq_t total = 1;                   /* the leading 1 in the formula */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum;

        if (i == 1)                    /* special case: S_1 = n */
            sum = (seq_t)n;
        else
        {
            seq_t base   = (seq_t)i;
            seq_t pow    = seq_pow(base, n + 1);   /* base^(n+1) */
            seq_t numer  = pow - base;            /* base^(n+1) - base */
            sum = numer / (base - 1);             /* geometric series sum */
        }

        total += sum * sum;          /* add (S_i)^2 to the total */
    }

    return total;
}

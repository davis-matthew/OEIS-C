/*
 * A118115  Partial sums of n concatenated n times.
 * https://oeis.org/A118115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} A000461(i) where A000461(i) = i *
 *            (10^(i*L)-1)/(10^L-1), L = number of decimal digits of i
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   digits, pow10, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the geometric‑series expression
   A000461(i) = i * (1 + 10^L + 10^(2L) + … + 10^((i-1)L)),
   avoiding the large intermediate 10^(i*L). */

#include "../utils/digits.h"   /* digits(idx_t) */
#include "../utils/pow10.h"    /* pow10(idx_t) */
#include "../utils/seq_one.h"  /* seq_one(void) */

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;          /* accumulated result */
    seq_t one = seq_one();         /* constant 1 */

    for (idx_t i = 1; i <= n; ++i) {
        idx_t L      = digits(i);            /* length in decimal digits */
        seq_t powL   = pow10(L);              /* 10^L */

        /* Compute the geometric sum  1 + 10^L + … + 10^((i-1)L) */
        seq_t mult      = one;               /* current power of 10^L    */
        seq_t sum_term  = one;               /* running sum: initially 1  */

        for (idx_t k = 1; k < i; ++k) {
            mult     = mult * powL;           /* 10^(k*L) */
            sum_term = sum_term + mult;       /* add to sum */
        }

        seq_t term = (seq_t)i * sum_term;    /* A000461(i) */
        sum = sum + term;                    /* accumulate into a(n) */
    }

    return sum;
}

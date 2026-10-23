/*
 * A133360  Number of surjections from an n-element set to a nine-element set.
 * https://oeis.org/A133360
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..9} (-1)^(9-k) * binomial(9,k) * k^n
 * Range:     u8 none, u16 none, u32 n <= 11, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;

    for (idx_t k = 1; k <= 9; ++k)
    {
        seq_t bin  = binom(9, k);                     /* C(9,k)          */
        seq_t powk = big_pow((seq_t)k, (seq_t)n);     /* k^n              */
        seq_t term = bin * powk;                      /* C(9,k)*k^n       */

        if (((9 - k) & 1U) == 0)                      /* (-1)^(9-k)  */
            pos += term;                             /* add positive   */
        else
            neg += term;                             /* add negative   */
    }

    return pos - neg;                                 /* final result  */
}

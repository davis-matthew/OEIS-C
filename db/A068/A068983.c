/*
 * A068983  a(n) = Sum_{k=0..n} (k^k-k!).
 * https://oeis.org/A068983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A068983: a(n) = Sum_{k=0..n} (k^k - k!)  (offset 1) */

#include <stdint.h>
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* 0! = 1, 0^0 is defined as 1 for this sequence */
    seq_t sum   = (seq_t)0;
    seq_t fact  = (seq_t)1;          /* previous factorial (k!) */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t powk;
        if (k == 0) {
            powk = (seq_t)1;         /* 0^0 = 1 */
        } else {
            powk = big_pow((seq_t)k, (seq_t)k);
            fact *= (seq_t)k;        /* k! = (k-1)! * k */
        }
        sum += powk - fact;
    }
    return sum;
}

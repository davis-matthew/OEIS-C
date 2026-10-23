/*
 * A038397  Concatenate first n squares in reverse order.
 * https://oeis.org/A038397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate the squares 1², 2², … ,n² in reverse order *
 *            (starting with n², then (n‑1)², …, 1²)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"      /* seq_t pow10(idx_t e);          */
#include "../utils/digit_len.h"  /* int digit_len(seq_t x);        */

seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t k = n; k > 0; --k)
    {
        seq_t kk      = (seq_t)k;          /* cast to seq_t for arithmetic   */
        seq_t square  = kk * kk;           /* k²                               */
        int   digits  = digit_len(square); /* number of decimal digits       */
        seq_t mult    = pow10((idx_t)digits); /* 10^digits                      */
        result = result * mult + square;   /* concatenate                     */
    }

    return result;
}

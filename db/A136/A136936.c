/*
 * A136936  Numbers k such that k and k^2 use only the digits 0, 3, 5 and 6.
 * https://oeis.org/A136936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by merging the two strictly increasing sequences
   6 · 10^m   and   60553006 · 10^m  (m ≥ 0).  The function
   returns the n‑th term (n ≥ 1), with a(1) = 0.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* Number of terms to generate after the initial 0. */
    idx_t   remain = n - 1;
    seq_t  a_val = (seq_t)6;
    seq_t  b_val = (seq_t)60553006;
    seq_t  result = 0;

    for (idx_t i = 0; i < remain; ++i)
    {
        if (a_val <= b_val)
        {
            result = a_val;
            a_val *= (seq_t)10;
        }
        else
        {
            result = b_val;
            b_val *= (seq_t)10;
        }
    }
    return result;
}

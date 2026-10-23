/*
 * A033714  Number of zeros in numbers 0 to 999..9 (n digits).
 * https://oeis.org/A033714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   a(n) = n*10^(n-1) - (10^n - 10) / 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n first.  We then derive 10^(n-1) from it.  */
    seq_t t_n   = pow10(n);
    seq_t t_n1  = t_n / (seq_t)10;                     /* 10^(n-1)  */

    /* (10^n - 10) / 9 is an integer.  */
    seq_t sub   = (t_n - (seq_t)10) / (seq_t)9;

    return ((seq_t)n) * t_n1 - sub;
}

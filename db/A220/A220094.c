/*
 * A220094  Sum of the n-digit base-ten numbers whose digits are nonzero.
 * https://oeis.org/A220094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5 * 9^(n-1) * (10^n - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* offset is 1, but guard against n == 0 */

    seq_t pow9   = pow_seq((seq_t)9,  (int)(n - 1));  /* 9^(n-1) */
    seq_t pow10n = pow_seq((seq_t)10, (int)n);        /* 10^n   */
    seq_t term   = pow10n - (seq_t)1;                 /* 10^n-1  */

    return ((seq_t)5 * pow9) * term;                  /* 5*9^(n-1)*(10^n-1) */
}

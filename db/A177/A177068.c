/*
 * A177068  a(n) = 10^n + n^10.
 * https://oeis.org/A177068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n + n^10
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow10, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t pow10n = pow10(n);          /* 10^n  */
    seq_t nseq   = (seq_t)n;          /* convert index to seq_t */
    seq_t n10    = pow_int(nseq, 10); /* n^10   */
    return pow10n + n10;              /* sum gives a(n) */
}

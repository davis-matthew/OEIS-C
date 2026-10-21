/*
 * A091693  a(n) = (n*10^n - n + 9)/9.
 * https://oeis.org/A091693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n*10^n - n + 9)/9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t pow_n = pow10(n);                    /* 10^n               */
    seq_t prod  = ((seq_t)n) * pow_n;          /* n * 10^n           */
    prod = prod - ((seq_t)n);                  /* n*10^n - n         */
    prod = prod + (seq_t)9;                    /* n*10^n - n + 9     */
    return prod / (seq_t)9;                    /* (n*10^n - n + 9)/9 */
}

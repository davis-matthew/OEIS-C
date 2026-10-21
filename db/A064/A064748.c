/*
 * A064748  a(n) = n*10^n + 1.
 * https://oeis.org/A064748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*10^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   int_pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_pow10.h"

seq_t a(idx_t n)
{
    /* compute 10^n */
    seq_t pow10n = int_pow10((unsigned)n);

    /* cast n to seq_t and multiply */
    seq_t n_s   = (seq_t)n;
    seq_t prod  = n_s * pow10n;

    /* finally add 1 */
    return prod + (seq_t)1;
}

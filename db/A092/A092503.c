/*
 * A092503  a(n) = n^floor(n/2).
 * https://oeis.org/A092503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^floor(n/2)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 47,
 *            big any n
 * Helpers:   _pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_pow_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;        /* 0^0 is defined as 1 for this sequence */
    idx_t k = n / 2;              /* floor(n/2) */
    seq_t base = (seq_t)n;        /* convert index to seq_t */
    return _pow_seq_t(base, k);   /* efficient exponentiation */
}

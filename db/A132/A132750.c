/*
 * A132750  A132749 * [1, 2, 3, ...] = A007318 * A065190.
 * https://oeis.org/A132750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0 a(n) = n*2^(n-1) + 2^n + 1 for n ≥ 1 (1)
 *            derived from the OEIS entry for A132750
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: 0^n is 0 for n≥1, so the -0^n term is omitted. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t pow2_n   = pow2(n);        /* 2^n    */
    seq_t pow2_nm1 = pow2(n - 1);    /* 2^(n-1) */

    /* n * 2^(n-1) + 2^n + 1  */
    seq_t res = ((seq_t)n) * pow2_nm1 + pow2_n + (seq_t)1;

    return res;
}

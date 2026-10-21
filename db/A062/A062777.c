/*
 * A062777  a(n) = 2^n - mu(n).
 * https://oeis.org/A062777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n - μ(n) (n ≥ 1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   mobius, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* 2^n part – pow2 expects an idx_t exponent */
    seq_t two_pow = pow2(n);

    /* Möbius part – μ(n) is returned as a seq_t (−1, 0, or 1) */
    seq_t mu = mobius(n);

    /* Result: 2^n − μ(n) */
    return two_pow - mu;
}

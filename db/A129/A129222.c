/*
 * A129222  Residues of the Lucas - Lehmer primality test for M(17) = 131071.
 * https://oeis.org/A129222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(0) = 4, a(n) = (a(n-1)^2 - 2) mod 131071 (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)131071;   /* 2^17 – 1 */
    seq_t val = (seq_t)4;              /* a(0)   */

    if (n == 0)
        return val;

    for (idx_t i = 1; i <= n; ++i) {
        /* square modulo MOD */
        seq_t tmp = val * val;
        tmp = tmp % MOD;

        /* subtract 2 modulo MOD */
        if (tmp >= (seq_t)2)
            val = tmp - (seq_t)2;
        else
            val = tmp + MOD - (seq_t)2;
    }

    return val;
}

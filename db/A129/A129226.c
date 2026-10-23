/*
 * A129226  Residues of the Lucas - Lehmer primality test for M(31) =
 *          2147483647.
 * https://oeis.org/A129226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 4
   a(n) = (a(n-1)^2 - 2) mod (2^31-1)  for n ≥ 1   */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Modulus 2^31 – 1 */
    const seq_t MOD = (seq_t)2147483647;

    seq_t cur = (seq_t)4;

    if (n == 0)
        return cur;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Square of the previous term */
        seq_t sq = cur * cur;
        /* Reduce modulo MOD */
        seq_t val = sq % MOD;
        /* Subtract 2 and wrap around MOD if necessary */
        val = (val + MOD - (seq_t)2) % MOD;
        cur = val;
    }
    return cur;
}

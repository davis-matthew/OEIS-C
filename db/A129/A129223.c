/*
 * A129223  Residues of the Lucas - Lehmer primality test for M(19) = 524287.
 * https://oeis.org/A129223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(0) = 4; for n>0, a(n) = (a(n-1)^2 - 2) mod 524287
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;

    const seq_t MOD = (seq_t)524287;
    seq_t val = (seq_t)4;

    for (idx_t i = 1; i <= n; ++i)
    {
        val = ((val * val) + MOD - 2) % MOD;
    }

    return val;
}

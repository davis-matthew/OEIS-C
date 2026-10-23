/*
 * A129219  Residues of the Lucas - Lehmer primality test for M(7) = 127.
 * https://oeis.org/A129219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(0) = 4; a(n) = (a(n-1)^2 - 2) mod 127, for n ≥ 1
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t mod = (seq_t)127;
    const seq_t two = (seq_t)2;
    seq_t val = (seq_t)4;

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute a(n-1)^2 mod 127 */
        seq_t sq = (val * val) % mod;

        /* Subtract 2 modulo 127 */
        if (sq < two) {
            sq += mod;
        }
        val = sq - two;
    }

    return val;
}

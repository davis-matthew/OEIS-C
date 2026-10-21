/*
 * A070661  a(n) = n^6 mod 38.
 * https://oeis.org/A070661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 38
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)38;
    seq_t n_mod = ((seq_t)n) % MOD;
    seq_t result = 1;
    for (int i = 0; i < 6; ++i) {
        result = (result * n_mod) % MOD;
    }
    return result;
}

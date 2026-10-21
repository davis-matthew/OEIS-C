/*
 * A070618  a(n) = n^5 mod 35.
 * https://oeis.org/A070618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 35
 * Range:     u8 n <= 1535, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Immediate modulus reduction of the index */
    const seq_t MOD = (seq_t)35;
    seq_t m = (seq_t)n % MOD;

    /* Compute m^5 modulo MOD */
    seq_t result = 1;
    int i;
    for (i = 0; i < 5; ++i)
    {
        result = (result * m) % MOD;
    }
    return result;
}

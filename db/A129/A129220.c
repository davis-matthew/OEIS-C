/*
 * A129220  Residues of the Lucas - Lehmer primality test for M(11) = 2047.
 * https://oeis.org/A129220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(0) = 4; a(n) = a(n-1)^2 - 2 mod 2047 for n>0
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t MOD = 2047;
    seq_t val = 4;
    for (idx_t i = 0; i < n; ++i)
    {
        unsigned long long prod = (unsigned long long)val * (unsigned long long)val;
        prod = (prod + 2045ULL) % 2047ULL;  /* (a*a - 2) mod 2047 */
        val = (seq_t)prod;
    }
    return val;
}

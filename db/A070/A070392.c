/*
 * A070392  a(n) = 6^n mod 11.
 * https://oeis.org/A070392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n mod 11
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 11;
    const seq_t BASE = 6;
    /* Use the fact that 6^10 ≡ 1 (mod 11).  Reduce exponent modulo 10. */
    idx_t e = n % 10;
    seq_t result = 1;
    for (idx_t i = 0; i < e; ++i)
        result = (result * BASE) % MOD;
    return result;
}

/*
 * A230024  Final nonzero digit of n^n in base 16.
 * https://oeis.org/A230024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ((n >> v2(n))^n mod 16) * 2^((v2(n)*n) mod 4) mod 16
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1; v2(n) is the exponent of 2 in n (t) */
    idx_t t = __builtin_ctzll((unsigned long long)n);

    /* odd part of n, modulo 16 suffices because we only need it mod 16 */
    seq_t u = ((seq_t)(n >> t)) & 15U;

    /* exponent for the odd part modulo 8 (phi(16)=8) */
    idx_t exp_mod = n & 7U;
    if (exp_mod == 0U)
        exp_mod = 8U;

    /* compute u^exp_mod modulo 16 */
    seq_t pow = 1;
    seq_t base = u & 15U;
    for (idx_t i = 0; i < exp_mod; ++i)
        pow = (pow * base) & 15U;

    /* remaining power of two after removing all full 16's */
    idx_t r = ((t & 3U) * (n & 3U)) & 3U;
    seq_t factor = 1U << r;
    pow = (pow * factor) & 15U;

    return pow;
}

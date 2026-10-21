/*
 * A231566  gcd(1,2n)*1^2n + ...+ gcd(i,2n)*i^2n + ... + gcd(2n,2n)*2n^2n (mod
 *          2n).
 * https://oeis.org/A231566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{2n} gcd(i, 2n) * i^(2n) (mod 2n)
 * Range:     u8 n <= 66, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* m = 2n is the modulus */
    idx_t m_idx = 2 * n;
    seq_t mod = (seq_t)m_idx;

    seq_t sum = 0;

    for (idx_t i = 1; i <= m_idx; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t gcd_val = gcd(i_seq, mod);
        seq_t pow_val = mod_pow(i_seq, (uint64_t)m_idx, mod);
        seq_t term = (gcd_val * pow_val) % mod;
        sum += term;
        sum %= mod;
    }

    return sum % mod;
}

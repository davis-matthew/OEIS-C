/*
 * A105222  Smallest integer m > 1 such that m^(n-1) == 1 (mod n).
 * https://oeis.org/A105222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Smallest integer m>1 such that m^(n-1) ≡ 1 (mod n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9789 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* fast modular exponentiation, works for all seq_t types */
static inline seq_t modpow_seq(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    base %= mod;
    while (exp > 0)
    {
        if (exp & 1)
            result = (base * result) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 2 */
    if (n == 1) return (seq_t)2;

    seq_t mod = (seq_t)n;
    seq_t exp = (seq_t)(n - 1);

    /* Search m from 2 upward until the residue is 1 */
    for (idx_t m = 2; ; ++m)
    {
        seq_t res = modpow_seq((seq_t)m, exp, mod);
        if (res == (seq_t)1)
            return (seq_t)m;
    }
}

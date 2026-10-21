/*
 * A065295  Number of values of s, 0 < s <= n-1, such that s^s == s (mod n).
 * https://oeis.org/A065295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = # { s | 0 < s <= n-1 and s^s ≡ s (mod n) }
 * Range:     u8 n <= 1004, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* modular exponentiation: computes (base^exp) mod mod
   base, mod are seq_t; exp is idx_t (uint64_t). */
static seq_t modpow(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1 % mod;          /* in case mod == 1 */
    seq_t b = base % mod;
    while (exp > 0) {
        if (exp & 1)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;   /* a(1) = 0 by definition */

    seq_t count = 0;
    seq_t mod = (seq_t)n;
    for (idx_t s = 1; s < n; ++s) {
        seq_t ss = (seq_t)s;
        /* test if s^s ≡ s (mod n) */
        if (modpow(ss, s, mod) == ss)
            ++count;
    }
    return count;
}

/*
 * A214739  Least m>0 such that n-m divides 2^(n-1)+m.
 * https://oeis.org/A214739
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - max{ d | d < n, d divides 2^(n-1)+n }
 * Range:     u8 n <= 259, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t modpow2(idx_t exp, idx_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1;
    uint64_t base = 2 % mod;
    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* undefined for n < 2 */
    for (idx_t d = n - 1; d > 0; --d) {
        seq_t powmod = modpow2(n - 1, d);
        seq_t s = (powmod + (n % d)) % d;
        if (s == 0)                      /* d divides 2^(n-1)+n */
            return n - d;                /* minimal m = n - d */
    }
    return 0;   /* unreachable, keeps compiler happy */
}

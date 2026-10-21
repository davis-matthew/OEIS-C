/*
 * A214718  Least m>0 such that 5^n-m and n-m are relatively prime.
 * https://oeis.org/A214718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimal m>0 such that gcd(5^n - m, n - m) = 1
 * Range:     u8 n <= 266, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Euclidean algorithm for seq_t (works for any seq_t supporting %). */
static seq_t my_gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* protect against 0 (not part of OEIS offset) */
        return 0;
    if (n == 1)          /* a(1)=2 is given in the OEIS entry */
        return 2;

    /* search m=1,2,... until n-m>0 and gcd(5^n-m, n-m)=1 */
    for (idx_t m = 1; m < n; ++m) {
        idx_t d = n - m;                /* d = n - m > 0 */
        seq_t mod = (seq_t)d;

        /* r = (5^n - m) mod d */
        seq_t powmod = mod_pow((seq_t)5, n, mod);
        seq_t r = powmod;
        seq_t m_mod = (seq_t)(m % d);
        if (r >= m_mod)
            r -= m_mod;
        else
            r = r + mod - m_mod;

        if (my_gcd(r, mod) == 1)
            return (seq_t)m;           /* smallest such m found */
    }

    /* In theory this line should never be reached for n>=2. */
    return (seq_t)n;
}

/*
 * A214054  Least m>0 such that n!!+m and n-m have a common divisor > 1.
 * https://oeis.org/A214054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m > 0 such that gcd(n!! + m, n - m) > 1, where n!!
 *            is the double factorial of n. In particular: - if n is odd, a(n)
 *            = 1, - if n is even and (n - 1) divides n!! + 1, a(n) = 1, -
 *            otherwise a(n) = 2 (m = 2 always works for even n)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* compute a(n) */
seq_t a(idx_t n)
{
    /* n == 1 returns 1 immediately */
    if ((n & 1) == 1)  /* n is odd */
        return (seq_t)1;

    /* n is even */
    if (n == 2)   /* handle the special case n == 2 cleanly */
        return (seq_t)2;

    /* n >= 4, even */
    seq_t mod = (seq_t)(n - 1);   /* n-1 is odd and >0 */
    seq_t prod = 1;

    /* compute n!! modulo (n-1) */
    for (idx_t i = n; i >= 2; i -= 2)
        prod = (prod * (seq_t)i) % mod;

    /* r = (n!! + 1) mod (n-1) */
    seq_t r = prod + 1;
    if (r == mod)
        r = 0;  /* shortcut for modulus ring */

    /* if gcd(r, n-1) > 1 then m=1 works */
    seq_t d = gcd(r, mod);
    if (d > 1)
        return (seq_t)1;
    else
        return (seq_t)2;
}

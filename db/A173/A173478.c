/*
 * A173478  a(n) = the smallest number ending in n-1 zeros divisible by n.
 * https://oeis.org/A173478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) = lcm(n, 10^(n-1))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

/* Euclidean algorithm for seq_t type */
static seq_t local_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t sn      = (seq_t)n;         /* n as seq_t */
    seq_t pow10i  = pow10(n - 1);     /* 10^(n-1) */
    seq_t g       = local_gcd(sn, pow10i);
    return (sn / g) * pow10i;        /* lcm = n/gcd * 10^(n-1) */
}

/*
 * A045895  Period length of pairs (a,b) where a has period 2n-2 and b has
 *          period n.
 * https://oeis.org/A045895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(2*n-2, n)
 * Range:     u8 n <= 12, u16 n <= 182, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1, but safe guard */
    seq_t nn = (seq_t)n;
    seq_t two_n_minus_two = (seq_t)2 * n - 2;

    /* gcd expects seq_t arguments; for n = 1, two_n_minus_two = 0 */
    seq_t g = gcd(nn, two_n_minus_two);

    /* lcm(2*n-2, n) = n * (2*n-2) / gcd(n, 2*n-2) */
    /* compute (nn / g) first to keep numbers small */
    return (nn / g) * two_n_minus_two;
}

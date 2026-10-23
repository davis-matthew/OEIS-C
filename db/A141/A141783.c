/*
 * A141783  Number of bracelets (turn over necklaces) with n beads: 1 blue, 12
 *          green, and r = n - 13 red.
 * https://oeis.org/A141783
 *
 * Offset:    13    (a(13) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (binom(n-1,12) + binom((n-2+n mod 2)/2,6)) / 2, n ≥ 13
 *            (a(n) = 0 for n < 13)
 * Range:     u8 n <= 15, u16 n <= 18, u32 n <= 34, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 13u)
        return 0;
    /* First binomial term: C(n-1,12) */
    seq_t t1 = binom(n - 1, 12);
    /* Second binomial term: C((n-2 + (n mod 2))/2, 6) */
    idx_t m = (n - 2u + (n & 1u)) / 2u;
    seq_t t2 = binom(m, 6);
    /* Sum and divide by 2 */
    return (t1 + t2) / 2;
}

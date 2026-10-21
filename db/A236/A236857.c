/*
 * A236857  Each n occurs the least common multiple (LCM) of {1, 2, ..., n} (=
 *          A003418(n)) times.
 * https://oeis.org/A236857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the least k such that Σ_{i=0}^{k} lcm(1..i) > n. Each
 *            integer i occurs lcm(1..i) times. The code computes the
 *            cumulative sum of LCMs incrementally until it exceeds n, then
 *            returns the current index i
 * Range:     u8 n <= 86, u16 n <= 6441, u32 n <= 6441, u64 n <= 6441,
 *            u128 n <= 6441, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t lcm = 1;   /* lcm(1..0) = 1 by definition */
    seq_t cum = 1;   /* cumulative sum up to current i */
    idx_t i = 0;

    while (cum <= n) {
        ++i;                                 /* next row index */
        seq_t g = gcd(lcm, (seq_t)i);        /* gcd(previous lcm, i) */
        lcm = (lcm / g) * (seq_t)i;          /* lcm(1..i) */
        cum += lcm;                          /* update cumulative sum */
    }

    return (seq_t)i;                          /* row containing position n */
}

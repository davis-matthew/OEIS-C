/*
 * A217367  a(n) = ((n+7) / gcd(n+7,4)) * (n / gcd(n,4)).
 * https://oeis.org/A217367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+7) / gcd(n+7,4)) * (n / gcd(n,4))
 * Range:     u8 n <= 21, u16 n <= 358, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute OEIS sequence A217367 */
seq_t a(idx_t n)
{
    seq_t n_val    = (seq_t)n;
    seq_t n_plus7  = n_val + (seq_t)7;
    seq_t g1       = gcd(n_plus7, (seq_t)4);
    seq_t g2       = gcd(n_val,       (seq_t)4);
    seq_t part1    = n_plus7 / g1;   /* (n+7)/gcd(n+7,4) */
    seq_t part2    = n_val   / g2;   /* n/gcd(n,4) */
    return part1 * part2;           /* product */
}

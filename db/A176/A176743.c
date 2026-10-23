/*
 * A176743  a(n) = gcd(A000217(n+1), A002378(n+2)).
 * https://oeis.org/A176743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd((n+1)*(n+2)/2 , (n+2)*(n+3))
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n + 1;
    seq_t n2 = (seq_t)n + 2;
    seq_t tri  = n1 * n2 / 2;
    seq_t pronic = n2 * ((seq_t)n + 3);
    return gcd(tri, pronic);
}

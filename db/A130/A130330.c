/*
 * A130330  Triangle read by rows, the matrix product A130321 * A000012, both
 *          taken as infinite lower triangular matrices.
 * https://oeis.org/A130330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 2^( r - c + 1 ) - 1, where * r = floor((sqrt(1+8*n) -
 *            1)/2), * c = n - r*(r+1)/2. * This implements the OEIS A130330
 *            sequence
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 8255, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute r = floor((sqrt(1+8*n)-1)/2) with 128‑bit safety */
    unsigned __int128 v = (unsigned __int128)1 + (unsigned __int128)8 * n;
    unsigned long long s = sqrtl((long double)v);
    while ((unsigned __int128)(s + 1) * (s + 1) <= v) ++s;
    while ((unsigned __int128)s * s > v) --s;
    idx_t r = (s - 1) / 2;

    /* Compute the column index c */
    unsigned __int128 prod = (unsigned __int128)r * (r + 1);
    prod /= 2;
    idx_t start = (idx_t)prod;
    idx_t c = n - start;

    /* Compute the value 2^(r - c + 1) - 1 */
    idx_t exp = r - c + 1;
    seq_t val = pow2(exp);
    val -= (seq_t)1;
    return val;
}

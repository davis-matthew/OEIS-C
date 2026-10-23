/*
 * A187948  [nr+kr]-[nr]-[kr], where r=(1+sqrt(5))/2, k=6, [ ]=floor.
 * https://oeis.org/A187948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊(n+6)φ⌋ - ⌊nφ⌋ - 9, φ = (1+√5)/2
 * Range:     u8 none, u16 n <= 51, u32 n <= 124, u64 n <= 124, u128 n <= 124,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t type */
    seq_t nn   = (seq_t)n;
    seq_t n6   = nn + (seq_t)6;

    /* floor(n * √5) = floor(√(5 * n²)) */
    seq_t t1   = (seq_t)5 * nn * nn;
    seq_t s1   = floor_sqrt(t1);
    seq_t floor_n_phi = (nn + s1) / 2;

    /* floor((n+6) * √5) = floor(√(5 * (n+6)²)) */
    seq_t t2   = (seq_t)5 * n6 * n6;
    seq_t s2   = floor_sqrt(t2);
    seq_t floor_n6_phi = (n6 + s2) / 2;

    /* a(n) = floor((n+6)φ) - floor(nφ) - floor(6φ) */
    seq_t result = floor_n6_phi - floor_n_phi - (seq_t)9;
    return result;
}

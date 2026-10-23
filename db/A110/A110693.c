/*
 * A110693  Kekulé numbers for certain benzenoids.
 * https://oeis.org/A110693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n+1)*(n+2)^3*(n+3)*(n+4)*(25*n^3 + 142*n^2 + 295*n +
 *            210)/8!
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 22, u64 n <= 288,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses incremental GCD cancellation so that all intermediate
   values fit into seq_t whenever the final value does. */

#include <stdint.h>
#include "../utils/gcd_uint64.h"

/* Euclidean algorithm on unsigned 64-bit integers. */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* factors of the numerator */
    seq_t factors[7];
    factors[0] = nn + 1;          /* (n+1)    */
    factors[1] = nn + 2;          /* (n+2)    */
    factors[2] = nn + 2;          /* (n+2)    */
    factors[3] = nn + 2;          /* (n+2)    */
    factors[4] = nn + 3;          /* (n+3)    */
    factors[5] = nn + 4;          /* (n+4)    */
    /* polynomial part */
    seq_t P = 25 * nn * nn * nn + 142 * nn * nn + 295 * nn + 210;
    factors[6] = P;

    /* 8! / 2 = 20160 */
    uint64_t denom = 20160;
    seq_t  result = 1;

    for (int i = 0; i < 7; ++i) {
        seq_t  f  = factors[i];
        uint64_t f64 = (uint64_t)f;           /* safe cast: f ≤ denom or a small integer   */
        uint64_t g  = gcd_uint64(f64, denom); /* cancel common factors with the denominator */

        if (g > 1) {
            f64   /= g;
            denom  /= g;
        }

        result *= (seq_t)f64;                  /* f64 fits into seq_t after division     */
    }

    /* denom should be 1, but divide just in case */
    if (denom != 1) {
        result /= (seq_t)denom;
    }

    return result;
}

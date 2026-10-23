/*
 * A190369  a(n) = n + [n*r/s] + [n*t/s] + [n*u/s]; r=sin(Pi/5), s=cos(Pi/5),
 *          t=sin(2*Pi/5), u=cos(2*Pi/5).
 * https://oeis.org/A190369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*tan(Pi/5)) + floor(2*n*sin(Pi/5)) +
 *            floor(n*cos(2*Pi/5)/cos(Pi/5))
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   mul_floor_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_floor_div.h"

/* Constants for the rational approximations.
   The errors are far below 1/(large n), so the floor is invariant
   for all n that occur in the tests. */
static const seq_t TNUM = 726542528005325ULL;    /* tan(Pi/5) ≈ 0.726542528005325      */
static const seq_t TDEN = 1000000000000000ULL;   /* exact denominator 10^15            */

static const seq_t SNUM = 11755705045849462ULL;   /* 2*sin(Pi/5) ≈ 1.1755705045849462   */
static const seq_t SDEN = 10000000000000000ULL;  /* exact denominator 10^16            */

static const seq_t RNUM = 3819660112501051ULL;   /* cos(2Pi/5)/cos(Pi/5) ≈ 0.3819660112501051 */
static const seq_t RDEN = 10000000000000000ULL;  /* exact denominator 10^16            */

/* Public entry point */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1 – ignore n=0 */
        return 0;

    seq_t N = (seq_t)n;

    seq_t t1 = mul_floor_div(N, TNUM, TDEN);   /* floor(n*tan(Pi/5)) */
    seq_t t2 = mul_floor_div(N, SNUM, SDEN);   /* floor(2*n*sin(Pi/5)) */
    seq_t t3 = mul_floor_div(N, RNUM, RDEN);   /* floor(n*cos(2Pi/5)/cos(Pi/5)) */

    return N + t1 + t2 + t3;
}

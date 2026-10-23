/*
 * A024558  a(n) = [ n/{n*sqrt(6)} ], where {x} := x - [ x ].
 * https://oeis.org/A024558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [ n/{n*sqrt(6)} ], where {x} := x - [x]
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Work in the integer domain only – no floating point */
    seq_t N = (seq_t)n;

    /* S = 6 * n^2 */
    seq_t S = (seq_t)6 * N * N;

    /* B = floor(sqrt(S))  – integer part of n*sqrt(6) */
    seq_t B = floor_sqrt(S);

    /* D = S - B^2    , 1 <= D <= 2*B (never zero because sqrt(6) is irrational) */
    seq_t D = S - B * B;

    /* a0  = floor( 2*n*B / D )  -- first guess for a(n) */
    seq_t twoN = N * 2;
    seq_t a0   = (twoN * B) / D;

    /* diff = (a0+1)*D - 2*n*B   ,  1 <= diff <= D-1  */
    seq_t diff = ((a0 + 1) * D) - (twoN * B);

    /* The true a(n) is a0 or a0+1.  It is a0+1 iff
       n*f  >= (a0+1)*D - 2*n*B  where f = {n*sqrt(6)}   */
    /* Use the bounds   D/(2B+1) < f < D/(2B)  to test the condition
       without computing the irrational f explicitly.           */
    seq_t left  = diff * (2 * B + 1);   /* diff * (2B+1)   */
    seq_t right = N * D;                /* n * D           */

    /* If diff*(2B+1) <= n*D the inequality holds and we add one. */
    if (left <= right) {
        return a0 + 1;
    } else {
        return a0;
    }
}

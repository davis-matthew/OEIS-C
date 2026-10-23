/*
 * A108599  Self-inverse integer permutation induced by Beatty sequences for e
 *          and e/(e-1).
 * https://oeis.org/A108599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   mul_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: 
 *   Let e be Euler's number, β = e/(e-1).  
 *   For n ≥ 1, find the unique k such that either  
 *     n = ⌊k·e⌋   or   n = ⌊k·β⌋.  
 *   Then a(n) = the other value:  
 *     a(⌊k·e⌋)   = ⌊k·β⌋,  
 *     a(⌊k·β⌋)   = ⌊k·e⌋.  
 *   The implementation uses high‑precision rational approximations 
 *   of e (P/Q) and β (P/(P−Q)) to compute the floors with integer arithmetic.
 */
#include <stdint.h>
#include "../utils/mul_div.h"

static const idx_t P  = 234286;   /* Approximation numerator of e   */
static const idx_t Q  = 86189;    /* Approximation denominator of e */
static const idx_t PB = P - Q;    /* (P−Q) = denominator of (P/Q)/(P/Q−1) = β */

seq_t a(idx_t n)
{
    /* Candidate k for the e‑Beatty sequence */
    idx_t k1 = mul_div(n + 1, Q, P);          /* floor((n+1)/e) */
    idx_t v1 = mul_div(k1, P, Q);            /* floor(k1*e) */

    if (v1 == n) {
        /* n is in the e Beatty sequence */
        idx_t v2 = mul_div(k1, P, PB);       /* floor(k1*β) */
        return (seq_t)v2;
    }

    /* n must be in the β Beatty sequence */
    idx_t k2 = mul_div(n + 1, PB, P);         /* floor((n+1)/β) */
    /* Optional sanity check: v2 should equal n */
    (void)mul_div(k2, P, PB); /* v2 = floor(k2*β) == n */

    idx_t v1b = mul_div(k2, P, Q);            /* floor(k2*e) */
    return (seq_t)v1b;
}

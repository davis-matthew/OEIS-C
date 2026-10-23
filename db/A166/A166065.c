/*
 * A166065  Triangle, read by rows, given by [0,1,1,0,0,0,0,0,0,0,...] DELTA
 *          [2,-1,0,0,0,0,0,0,0,...] where DELTA is the operator defined in
 *          A084938.
 * https://oeis.org/A166065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 for n = 0; otherwise * let m = floor((sqrt(8n+1)-1)/2)
 *            (the largest m with T(m)=m(m+1)/2 ≤ n), * let i = n - T(m). * if
 *            i == 0 (i.e., n is a triangular number >0) then a(n)=0, *
 *            otherwise let exp = m - i; if exp < 1 set exp = 1. * Then a(n) =
 *            2^exp
 * Range:     u8 n <= 7, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"          /* seq_t pow2(idx_t) */
#include "../utils/floor_sqrt.h"    /* seq_t floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) = 1 */
        return (seq_t)1;

    /* find   m = floor((sqrt(8n+1)-1)/2)  */
    seq_t t = (seq_t)8 * (seq_t)n + 1;      /* 8*n+1 (may need >64‑bit)   */
    seq_t sq = floor_sqrt(t);               /* floor(sqrt(8n+1))          */
    idx_t m = (idx_t)((sq - 1) / 2);        /* largest m with T(m) ≤ n    */

    /* triangular number T(m) = m(m+1)/2  */
    seq_t t_m = (seq_t)m * ((seq_t)m + 1) / 2;
    idx_t i = n - (idx_t)t_m;               /* distance to the next triangle */

    if (i == 0)                /* n is a triangular number (≥1) */
        return (seq_t)0;

    idx_t exp = m - i;         /* exponent for 2 */
    if (exp < 1)
        exp = 1;
    return pow2(exp);          /* 2^exp */
}

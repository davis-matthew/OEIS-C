/*
 * A158497  Triangle T(n,k) formed by the coordination sequences and the
 *          number of leaves for trees.
 * https://oeis.org/A158497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 75, u64 n <= 168,
 *            u128 n <= 432, big any n
 * Helpers:   int_sqrt, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A158497: a(n) = 1 if n is a triangular number; otherwise
 * a(n) = m * (m-1)^(n - T_m), where T_m = m(m+1)/2 is the largest triangular
 * number ≤ n. */
#include <stdint.h>
#include "../utils/int_sqrt.h"   /* int_sqrt(idx_t)    */
#include "../utils/pow_uint.h"   /* pow_uint(uint64_t, idx_t) */

seq_t a(idx_t n)
{
    if (n == 0) return 1;                /* a(0) = 1 */

    /* largest integer m such that m(m+1)/2 ≤ n */
    idx_t s = int_sqrt(1 + 8 * n);        /* floor sqrt(1 + 8n) */
    idx_t m = (s - 1) / 2;                /* floor ((sqrt(1+8n)-1)/2) */

    seq_t Tm = (seq_t)m * (seq_t)(m + 1) / 2;  /* triangular number T_m */

    if ((seq_t)n == Tm) return 1;        /* n is triangular */

    /* distance from T_m */
    idx_t d = n - (idx_t)Tm;             /* 1 ≤ d ≤ m */

    /* (m-1)^(d-1) */
    seq_t pow = pow_uint((uint64_t)(m - 1), d - 1);

    return (seq_t)m * pow;
}

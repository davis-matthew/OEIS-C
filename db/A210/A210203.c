/*
 * A210203  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210204; see the Formula section.
 * https://oeis.org/A210203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the (global) coefficient encountered while enumerating,
 *            row by row, the coefficients of the polynomials u(k,x) defined
 *            by u(1,x)=1 , v(1,x)=1 u(k,x)=u(k-1,x)+v(k-1,x)+1 (k≥2)
 *            v(k,x)=(x+1)u(k-1,x)+(x+1)v(k-1,x)+1 The nth term is the
 *            coefficient of u(r,x) where r is the unique integer satis...
 * Range:     u8 n <= 17, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
/* The following utility is available for seq_t arithmetic:
   seq_pow(seq_t base, idx_t exp)         – exponentiation by squaring
   The seq_t type and idx_t type are supplied by the build.        */

seq_t a(idx_t n) {
    if (n == 0)                /* undefined, offset is 1 */
        return (seq_t)0;

    if (n == 1)                /* first row, single coefficient 1 */
        return (seq_t)1;

    /* Find the row r such that 1 + r*(r-1)/2 ≥ n. */
    idx_t target = 2 * (n - 1);            /* compare r*(r-1) with target */
    idx_t low   = 2;
    idx_t high  = target + 2;              /* large enough upper bound */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 prod = (__int128)mid * (mid - 1);   /* r*(r-1) */
        if (prod >= (__int128)target)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                         /* row number, r ≥ 2 */

    /* Offset i within the row (0‑based). */
    idx_t prev_sum = 1 + (r - 1) * (r - 2) / 2;
    idx_t i = n - prev_sum - 1;             /* coefficient index in row */

    /* Exponent for powers of (x+2). */
    idx_t exp = r - 1;

    /* Compute 2^exp. */
    seq_t pow2 = (seq_t)1;
    for (idx_t k = 0; k < exp; ++k)
        pow2 *= (seq_t)2;          /* repeated doubling */

    /* Q0 = 2^exp – 1. */
    seq_t Q_prev = pow2 - (seq_t)1;

    if (i == 0)                     /* constant term */
        return (seq_t)2 * Q_prev + (seq_t)1;

    /* Compute successive Q_j. */
    seq_t binom = (seq_t)1;         /* C(exp, 0) */
    seq_t pow2_curr = pow2;         /* 2^(exp - (j-1))    */
    for (idx_t j = 1; j <= i; ++j) {
        pow2_curr /= (seq_t)2;                                   /* halve */
        binom = (binom * (seq_t)(exp - (j - 1))) / (seq_t)j;    /* C(exp,j) */
        seq_t Pj = binom * pow2_curr;                           /* P_j */
        seq_t Q_curr = Pj - Q_prev;
        if (j == i)
            return (seq_t)2 * Q_curr;        /* coefficient */
        Q_prev = Q_curr;
    }
    /* Piecemeal loop should return before reaching here. */
    return (seq_t)0;
}

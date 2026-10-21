/*
 * A224429  The Wiener index of the dendrimer D_1[n], defined pictorially in
 *          the A. R. Ashrafi et al. reference.
 * https://oeis.org/A224429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 7, u64 n <= 22, u128 n <= 54,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A224429 (Wiener index of dendrimer D_1[n])
    Formula: a(n) = -2655 + 26907*2^n + 16245*n*4^n -24168*4^n -570*n*2^n
    The function evaluates this closed-form expression for any non‑negative n.
*/
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Powers of two
       2^n  and  4^n = 2^(2n)
    */
    seq_t pow2_n   = pow2(n);
    idx_t two_n    = n + n;          /* 2*n, safe for idx_t up to 2^63-1 */
    seq_t pow4_n   = pow2(two_n);    /* 4^n = 2^(2n) */

    /* Individual terms of the formula */
    seq_t term1 = (seq_t)26907 * pow2_n;
    seq_t term2 = (seq_t)16245 * (seq_t)n * pow4_n;
    seq_t term3 = (seq_t)24168 * pow4_n;
    seq_t term4 = (seq_t)570   * (seq_t)n * pow2_n;

    /* Assemble the result, keeping the expression positive
       until the final subtraction of the constant 2655
    */
    seq_t result = 0;
    result += term1;
    result += term2;
    result -= term3;
    result -= term4;
    result -= 2655;

    return result;
}

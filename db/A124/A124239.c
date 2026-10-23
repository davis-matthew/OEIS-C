/*
 * A124239  a(n) = Sum_{k=1..n} Sum_{m=1..n} (2*k - 1)^m.
 * https://oeis.org/A124239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} Sum_{m=1..n} (2*k - 1)^m
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t total = 0;

    /* k = 1 contributes n * 1^m = n  */
    total = (seq_t)n;

    for (idx_t k = 2; k <= n; ++k) {
        seq_t r = 2 * (seq_t)k - 1;                /* odd base  */
        seq_t power = big_pow(r, (seq_t)n);        /* r^n      */
        seq_t term = power - 1;                   /* r^n – 1  */
        term /= 2 * ((seq_t)(k - 1));              /* divide by (r‑1) = 2*(k‑1)  */
        term *= r;                                 /* multiply by r  */
        total += term;                             /* accumulate   */
    }

    return total;
}

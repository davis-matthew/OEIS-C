/*
 * A032260  Number of n X n (0,1) matrices such that each row and each column
 *          is nondecreasing or nonincreasing.
 * https://oeis.org/A032260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*(binomial(2*n, n)-n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a multiplicative computation of the binomial coefficient.
   It works for any seq_t that supports integer arithmetic and division
   by small integers (as required for the arbitrary‑precision build). */
#include <stdint.h>

/* Internal helper: compute C(2*n, n) */
static seq_t binom_2n_n(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        res = res * ((seq_t)n + (seq_t)i); /* multiply by (n + i)   */
        res = res / (seq_t)i;              /* divide by i (exact) */
    }
    return res;
}

/* Sequence A032260 */
seq_t a(idx_t n)
{
    /* a(1) is the first term; n==0 is out of range. */
    if (n == 0)
        return 0;

    seq_t bin = binom_2n_n(n);             /* C(2n, n)          */
    seq_t sub = bin - (seq_t)n;            /* C(2n, n) – n      */
    seq_t result = (seq_t)2 * (seq_t)n * sub; /* 2*n*(C- n)        */
    return result;
}

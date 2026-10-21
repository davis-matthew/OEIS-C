/*
 * A090667  Number of compositions of 3n with each part less than or equal to
 *          n.
 * https://oeis.org/A090667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(3n-1) - (2n+1)*2^(2n-2) + (n+2)*(n-1)*2^(n-4) for n>0,
 *            a(0) = 1. Computed using only integer arithmetic, no
 *            floating‑point
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Helper: compute 2^exp for non‑negative exp.  Implemented as repeated
   multiplication by 2, which works for arbitrary‑precision seq_t. */
static seq_t pow2(idx_t exp)
{
    seq_t r = 1;
    seq_t two = 2;
    for (idx_t i = 0; i < exp; ++i)
        r *= two;          /* r = r * 2 */
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* --- term1 = 2^(3n-1) --------------------------------------- */
    idx_t exp1 = 3*n - 1;
    seq_t term1 = pow2(exp1);

    /* --- term2 = (2n+1) * 2^(2n-2) ------------------------------ */
    idx_t exp2 = 2*n - 2;
    seq_t term2 = (seq_t)(2*n + 1) * pow2(exp2);

    /* --- term3 = (n+2)*(n-1)*2^(n-4) --------------------------- */
    seq_t num3 = (seq_t)(n + 2) * (seq_t)(n - 1);
    seq_t term3;
    if (n >= 4) {
        /* exponent non‑negative */
        term3 = num3 * pow2(n - 4);
    } else {
        /* exponent negative: divide by 2^(4-n) */
        seq_t div = pow2(4 - n);
        term3 = num3 / div;   /* exact division */
    }

    return term1 - term2 + term3;
}

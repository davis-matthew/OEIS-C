/*
 * A212928  Number of nX6 0..5 arrays with no column j greater than column j-1
 *          in all rows.
 * https://oeis.org/A212928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 46656^n -5*19440^n -1 -3*540^n -6*1800^n +6*8100^n
 *            -3375^n +400^n +4*4320^n +2*225^n +2*36^n (n ≥ 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* seq_t and idx_t are defined by the build.  The function relies only on
   arithmetic on seq_t and on the provided big_pow routine. */
seq_t a(idx_t n)
{
    /* Base values in the formula, all fit in the native integer types.   */
    static const unsigned int base_vals[10] = {
        46656,   /* 6^6          */
        19440,   /* 19440        */
        540,     /* 540          */
        1800,    /* 1800         */
        8100,    /* 8100         */
        3375,    /* 3375         */
        400,     /* 400          */
        4320,    /* 4320         */
        225,     /* 225          */
        36       /* 36           */
    };

    /* Corresponding integer coefficients (positive or negative).   */
    static const int coeffs[10] = {
        1,   /* +46656^n*/
        -5,  /* -5*19440^n*/
        -3,  /* -3*540^n*/
        -6,  /* -6*1800^n*/
        6,   /* +6*8100^n*/
        -1,  /* -3375^n*/
        1,   /* +400^n*/
        4,   /* +4*4320^n*/
        2,   /* +2*225^n*/
        2    /* +2*36^n*/
    };

    seq_t result = 0;          /* final answer                       */
    seq_t exp = (seq_t)n;       /* exponent part                       */

    for (unsigned i = 0; i < 10; ++i) {
        seq_t power = big_pow((seq_t)base_vals[i], exp);
        if (coeffs[i] >= 0) {
            /* Add positive contributions.                       */
            result += power * (seq_t)coeffs[i];
        } else {
            /* Subtract negative contributions.                   */
            result -= power * (seq_t)(-coeffs[i]);
        }
    }

    /* Subtract the constant term -1 of the formula.               */
    result -= 1;

    return result;
}

/*
 * A159711  Number of permutations of 1..n arranged in a circle with exactly 3
 *          local maxima.
 * https://oeis.org/A159711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/3*2^(-6+n)*n*(15+3*2^(1+n)+3^n-3*(8+2^n)*n+6*n^2) for
 *            n>5, a(n)=0 for n ≤5
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n <= 5) return (seq_t)0;

    seq_t nn        = (seq_t)n;
    seq_t two_n      = pow2((idx_t)n);          /* 2^n            */
    seq_t two_n_plus1= pow2((idx_t)(n+1));      /* 2^(n+1)        */
    seq_t two_n_minus6= pow2((idx_t)(n-6));     /* 2^(n-6)        */
    seq_t three_n    = pow3((idx_t)n);          /* 3^n            */

    /* Compute the expression inside the brackets */
    seq_t part1 = (seq_t)15;
    seq_t part2 = (seq_t)3 * two_n_plus1;               /* 3*2^(n+1)      */
    seq_t part3 = three_n;                                /* 3^n            */
    seq_t part4 = (seq_t)3 * ((seq_t)8 + two_n) * nn;     /* 3*(8+2^n)*n    */
    seq_t part5 = (seq_t)6 * nn * nn;                     /* 6*n^2          */

    seq_t inside = part1 + part2 + part3 - part4 + part5;

    /* Multiply n * inside * 2^(n-6) and divide by 3 */
    seq_t numerator = nn * inside * two_n_minus6;
    seq_t result    = numerator / (seq_t)3;

    return result;
}

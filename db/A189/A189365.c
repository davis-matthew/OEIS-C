/*
 * A189365  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=sqrt(2), t=(1+sqrt(5))/2.
 * https://oeis.org/A189365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/√2⌋ + ⌊n·((1+√5)/2)/√2⌋ (r = 1, s = √2, t =
 *            (1+√5)/2)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 9110, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to the integer type used for sequence values. */
    seq_t sn = (seq_t)n;

    /* -----  Compute ⌊n/√2⌋  -----------------------------------------
       ⌊n/√2⌋ = ⌊(n·√2)/2⌋.  Since √2 * n = √(2·n²),
       we can obtain the integer part by taking the integer square root
       of 2·n² and then dividing by two. */
    seq_t n2 = sn * sn;           /* n²   */
    seq_t m1 = n2 * 2;            /* 2·n² */
    seq_t a_sqrt2 = floor_sqrt(m1);/* ⌊√(2·n²)⌋ */
    seq_t term1 = a_sqrt2 / 2;     /* ⌊n/√2⌋ */

    /* -----  Compute ⌊n·C⌋  -----------------------------------------
       C = (1+√5)/(2√2) = (√2+√10)/4.
       Let A = ⌊√(2·n²)⌋,  B = ⌊√(10·n²)⌋.
       Then ⌊n·C⌋ = ⌊(A+B)/4⌋  or  ⌊(A+B)/4⌋ + 1 when
       3 ≤ (A+B) mod 4  and  √(2·n²)+√(10·n²) ≥ A+B+1.
       This condition is tested exactly using integer arithmetic. */

    seq_t m2 = n2 * 10;              /* 10·n²   */
    seq_t a_sqrt10 = floor_sqrt(m2); /* ⌊√(10·n²)⌋ */

    seq_t sum_ab = a_sqrt2 + a_sqrt10;
    seq_t q = sum_ab / 4;            /* base value ⌊(A+B)/4⌋ */
    seq_t r = sum_ab % 4;            /* (A+B) mod 4   */
    seq_t term2 = q;

    if (r == 3) {
        /* Candidate for adding one.  Let w = A + B + 1. */
        seq_t w = sum_ab + 1;
        seq_t w2 = w * w;                /* w² */
        seq_t twelve_n2 = 12 * n2;        /* 12·n² */

        /* If w² ≤ 12·n²  →  w² − 12·n² ≤ 0, hence
           √(2·n²)+√(10·n²) ≥ w and the extra 1 is required. */
        if (w2 <= twelve_n2) {
            term2 = q + 1;
        } else {
            /* Else we need to test whether
               w² − 12·n² ≤ 2·n²·√20.
               Squaring both sides gives
               (w² − 12·n²)² ≤ 80·n⁴.          */
            seq_t d = w2 - twelve_n2;     /* positive */
            seq_t d2 = d * d;
            seq_t n4 = n2 * n2;
            seq_t rhs = 80 * n4;          /* 80·n⁴ */
            if (d2 <= rhs) {
                term2 = q + 1;
            }
        }
    }

    /* Sum the components to get a(n). */
    return sn + term1 + term2;
}

/*
 * A187391  Floor(r*n), where r=1+sqrt(8)+sqrt(7); complement of A187392.
 * https://oeis.org/A187391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (1 + sqrt(8) + sqrt(7)) * n )
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = n + floor( sqrt(8*n^2) ) + floor( sqrt(7*n^2) )
 *            + carry, where carry = 1 if the fractional parts
 *            of sqrt(8*n^2) and sqrt(7*n^2) sum to at least 1,
 *            otherwise carry = 0. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defined only for n >= 1 */

    /* n² */
    seq_t n_st = (seq_t)n;
    seq_t n2   = n_st * n_st;

    /* A = 8*n² , B = 7*n² */
    seq_t A = n2 * (seq_t)8;
    seq_t B = n2 * (seq_t)7;

    /* floor(sqrt(A)) and floor(sqrt(B)) */
    seq_t a_floor = floor_sqrt(A);
    seq_t b_floor = floor_sqrt(B);

    /* M = floor(sqrt(A)) + floor(sqrt(B)) + 1 */
    seq_t M = a_floor + b_floor + (seq_t)1;

    /* sumAB = A + B, M² = M * M */
    seq_t sumAB = A + B;
    seq_t M2    = M * M;

    /* Determine the carry */
    int carry_flag = 0;
    if (M2 < sumAB) {
        /* sqrt(A)+sqrt(B) < M cannot happen, but if it does carry = 1 */
        carry_flag = 1;
    } else {
        /* R = M² - (A + B)  (non‑negative) */
        seq_t R    = M2 - sumAB;
        /* left = 4 * A * B  = 4 * (8*n²) * (7*n²) = 56 * 4 * n⁴ = 224 * n⁴ */
        seq_t AB   = A * B;
        seq_t left = AB * (seq_t)4;
        /* right = R² */
        seq_t right = R * R;
        /* carry = 1 if 4*AB >= R²    (equivalent to 2*sqrt(AB) >= R)   */
        if (left >= right) carry_flag = 1;
    }

    seq_t floorSum = a_floor + b_floor;
    if (carry_flag) floorSum += (seq_t)1;

    /* a(n) = n + floorSum */
    return n_st + floorSum;
}

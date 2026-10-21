/*
 * A087066  a(n) = Sum_{k>=0} floor(n*(r^k)), where r = sqrt(5)-2.
 * https://oeis.org/A087066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=0} floor(n*(sqrt(5)-2)^k) (r = sqrt(5)-2)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root for 128‑bit values: floor(sqrt(x)) with x <= 2^128-1 */
static inline unsigned long long isqrt_uint128(unsigned __int128 x)
{
    unsigned long long r = 0;
    for (int i = 63; i >= 0; --i) {
        unsigned long long bit = 1ULL << i;
        unsigned long long cand = r | bit;
        if ((__int128)cand * cand <= x) r = cand;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* a_n and b_n are the integer coefficients of the expression
       n*(sqrt(5)-2)^k = a_n + b_n*sqrt(5)   */
    int64_t a_coeff = (int64_t)n;   /* a_0 = n, b_0 = 0 */
    int64_t b_coeff = 0;

    seq_t sum = 0;

    while (true) {
        unsigned long long floor_abs = 0;
        if (b_coeff != 0) {
            unsigned long long abs_b = (b_coeff >= 0) ? (unsigned long long)b_coeff
                                                     : (unsigned long long)(-b_coeff);
            unsigned __int128 sq = (unsigned __int128)abs_b * abs_b;
            sq *= 5;                            /* sqrt(5)*abs_b  -> sqrt(5*abs_b^2) */
            floor_abs = isqrt_uint128(sq);      /* floor(abs_b * sqrt(5)) */
        }

        /* term = a_coeff + sign(b_coeff) * floor_abs (+1 when b negative) */
        __int128 term_val = (__int128)a_coeff;
        if (b_coeff >= 0) {
            term_val += (__int128)floor_abs;
        } else {
            term_val -= (__int128)(floor_abs + 1);
        }

        seq_t term = (seq_t)term_val;          /* term >= 0 for all k */
        if (term == 0) break;                 /* stop when floor becomes zero */
        sum += term;

        /* compute next coefficients: multiply (a + b*sqrt5) by (sqrt5-2) */
        int64_t a_next = 5 * b_coeff - 2 * a_coeff;
        int64_t b_next = a_coeff - 2 * b_coeff;
        a_coeff = a_next;
        b_coeff = b_next;
    }

    return sum;
}

/*
 * A188741  Number of nX3 binary arrays without the pattern 1 1 1 diagonally,
 *          vertically or antidiagonally.
 * https://oeis.org/A188741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A188741: Number of n×3 binary arrays without a 3×1 block of ones
 * (diagonal, vertical or antidiagonal).
 *
 * Recurrence (order 16):
 *   a(n) = 2*a(n-1) +9*a(n-2) +50*a(n-3) +55*a(n-4) +37*a(n-5)
 *          -108*a(n-6) -111*a(n-7) +89*a(n-8) +27*a(n-9)
 *          -28*a(n-10) -5*a(n-11) -48*a(n-12) -28*a(n-13)
 *          -17*a(n-14) -4*a(n-15) -a(n-16)
 *
 * Initial terms for n = 1 … 16:
 *   8, 64, 292, 1651, 9504, 52072, 289776, 1617326,
 *   8992115, 50039730, 278556885, 1550225927, 8627663414,
 *   48018101493, 267244802833, 1487352476813
 *
 * The function below evaluates a(n) for any n ≥ 1, memoizing the
 * sequence up to the largest n requested.  It uses only integer
 * arithmetic and supports arbitrary‑precision seq_t types.
 */

#include <stdlib.h>

/* Multiply a seq_t by a small unsigned integer (≤ 111). */
static seq_t mul_small_seq(seq_t x, unsigned int m)
{
    seq_t result = 0;
    seq_t factor = x;
    while (m) {
        if (m & 1U) result += factor;
        factor <<= 1;
        m >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Recurrence coefficients: c[0] = coeff of a(n-1) ... c[15] = coeff of a(n-16) */
    static const signed int coeff[16] = {
        2, 9, 50, 55, 37, -108, -111, 89,
        27, -28, -5, -48, -28, -17, -4, -1
    };
    /* Initial values a(1)…a(16) */
    static const seq_t init[16] = {
        8ULL, 64ULL, 292ULL, 1651ULL, 9504ULL,
        52072ULL, 289776ULL, 1617326ULL, 8992115ULL,
        50039730ULL, 278556885ULL, 1550225927ULL,
        8627663414ULL, 48018101493ULL, 267244802833ULL,
        1487352476813ULL
    };

    if (n == 0) return 0;               /* out of range (unused) */

    /* Return known initial values directly. */
    if (n <= 16U) {
        return init[n - 1];
    }

    /* Static cache of computed terms: buf[i] = a(i+1) for i ≥ 0. */
    static seq_t *buf = NULL;
    static idx_t buf_size = 0;

    /* Initialise cache on first call. */
    if (buf == NULL) {
        buf_size = 16U;
        buf = (seq_t*)malloc(sizeof(seq_t) * buf_size);
        for (idx_t i = 0; i < 16U; ++i) {
            buf[i] = init[i];
        }
    }

    /* Extend cache up to required n. */
    if (n > buf_size) {
        /* Grow buffer to accommodate n terms. */
        if (n > buf_size * 2U) {
            buf_size = n;
        } else {
            buf_size = buf_size * 2U;
        }
        buf = (seq_t*)realloc(buf, sizeof(seq_t) * buf_size);
    }

    /* Compute missing terms from current size up to n. */
    for (idx_t k = 16U; k < n; ++k) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int t = 0; t < 16; ++t) {
            /* a(k+1 - t - 1) = a(k - t) corresponds to buf[k - t - 1] */
            idx_t idx = k - 1U - (idx_t)t;
            seq_t term = mul_small_seq(buf[idx], (unsigned int)(coeff[t] < 0 ? -coeff[t] : coeff[t]));
            if (coeff[t] >= 0) {
                sum_pos += term;
            } else {
                sum_neg += term;
            }
        }
        buf[k] = sum_pos - sum_neg; /* sum_pos ≥ sum_neg for valid recurrence */
    }

    return buf[n - 1];
}

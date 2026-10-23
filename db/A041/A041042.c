/*
 * A041042  Numerators of continued fraction convergents to sqrt(27).
 * https://oeis.org/A041042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=5, a(1)=26, a(n)=5*a(n-1)+a(n-2) if n is odd,
 *            a(n)=10*a(n-1)+a(n-2) if n is even. This follows from the
 *            continued‑fraction expansion of sqrt(27)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   mat_mul11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the numerator of the n‑th convergent to sqrt(27). */
#include <stdint.h>
#include "../utils/mat_mul11.h"

typedef uint64_t idx_t;          /* supplied by the build system */

/* 2×2 matrix multiplication on seq_t. */
static inline void mat_mul(seq_t *c11, seq_t *c12,
                           seq_t *c21, seq_t *c22,
                           const seq_t a11, const seq_t a12,
                           const seq_t a21, const seq_t a22,
                           const seq_t b11, const seq_t b12,
                           const seq_t b21, const seq_t b22)
{
    *c11 = a11 * b11 + a12 * b21;
    *c12 = a11 * b12 + a12 * b22;
    *c21 = a21 * b11 + a22 * b21;
    *c22 = a21 * b12 + a22 * b22;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;     /* a(0) */
    if (n == 1) return (seq_t)26;    /* a(1) */

    /* M = [[51,10],[5,1]]  (product of one odd and one even step) */
    seq_t m11 = (seq_t)51, m12 = (seq_t)10;
    seq_t m21 = (seq_t)5,  m22 = (seq_t)1;

    /* identity matrix */
    seq_t r11 = (seq_t)1, r12 = (seq_t)0;
    seq_t r21 = (seq_t)0, r22 = (seq_t)1;

    idx_t k = n >> 1;   /* number of full 2‑step blocks */

    /* binary exponentiation of M^k */
    while (k) {
        if (k & 1) {
            seq_t t11, t12, t21, t22;
            mat_mul(&t11, &t12, &t21, &t22,
                    r11, r12, r21, r22,
                    m11, m12, m21, m22);
            r11 = t11; r12 = t12; r21 = t21; r22 = t22;
        }
        seq_t t11, t12, t21, t22;
        mat_mul(&t11, &t12, &t21, &t22,
                m11, m12, m21, m22,
                m11, m12, m21, m22);
        m11 = t11; m12 = t12; m21 = t21; m22 = t22;
        k >>= 1;
    }

    /* Starting vector v0 = [p0, p_-1] = [5, 1] */
    seq_t p_even  = r11 * (seq_t)5 + r12 * (seq_t)1;   /* p_{2k} */
    seq_t p_prev  = r21 * (seq_t)5 + r22 * (seq_t)1;   /* p_{2k-1} */

    if ((n & 1) == 0)   /* even n = 2k */
        return p_even;
    else                /* odd n = 2k+1 */
        return (seq_t)5 * p_even + p_prev;
}

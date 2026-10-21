/*
 * A234950  Borel's triangle read by rows: T(n,k) = Sum_{s=k..n}
 *          binomial(s,k)*C(n,s), where C(n,s) is an entry in Catalan's
 *          triangle A009766.
 * https://oeis.org/A234950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 108, u64 n <= 333,
 *            u128 n <= 1146, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = T(r,k) where 
 *     r = ⌊(√(8n+1) - 1)/2⌋         (row index)
 *     k = n - r(r+1)/2             (column index)
 * and T(r,k) = 2·C(2r+1,r)·(r-k+1)·C(r+1,k) / ((r+k+1)(r+k+2))
 *
 * The function uses only integer arithmetic and the supplied binomial(n,k).
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

/* Greatest common divisor for seq_t values */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Find row r and column k from the linear index n */
    idx_t val = (idx_t)8 * n + (idx_t)1;
    idx_t s = (idx_t)0;
    /* Simple integer square‑root loop (n ≤ 10000 ⇒ s ≤ 300) */
    while ((s + (idx_t)1) * (s + (idx_t)1) <= val) {
        ++s;
    }
    idx_t r = (s - (idx_t)1) / (idx_t)2;
    idx_t prev_cnt = r * (r + (idx_t)1) / (idx_t)2;
    idx_t k = n - prev_cnt;

    /* Numerator factors */
    seq_t num[4];
    num[0] = (seq_t)2;
    num[1] = binomial((idx_t)2 * r + (idx_t)1, r);
    num[2] = (seq_t)(r - k + (idx_t)1);
    num[3] = binomial(r + (idx_t)1, k);

    /* Denominator factors */
    seq_t den[2];
    den[0] = (seq_t)(r + k + (idx_t)1);
    den[1] = (seq_t)(r + k + (idx_t)2);

    /* Cancel common factors between numerators and denominators */
    for (idx_t i = 0; i < 4; ++i) {
        seq_t x = num[i];
        for (idx_t j = 0; j < 2; ++j) {
            seq_t g = gcd_seq(x, den[j]);
            if (g != (seq_t)1) {
                x /= g;
                den[j] /= g;
            }
        }
        num[i] = x;
    }

    /* The denominators should now be 1 (division exact) */
    /* Multiply the remaining numerators */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < 4; ++i) {
        result *= num[i];
    }

    return result;
}

/*
 * A015586  Inverse of 1577th cyclotomic polynomial.
 * https://oeis.org/A015586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of xⁿ in the formal power series 1/Φ₁₅₇₇(x),
 *            thus a(n) is periodic with period 1577
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* constants for the 1577‑th cyclotomic polynomial */
#define N 1577
#define PHI_DEG 1476          /* φ(1577) = (19‑1)(83‑1) = 1476 */
#define PHI_SIZE (PHI_DEG + 1)
#define A_SIZE N              /* period length */

/* static buffers – built once on first call */
static bool ready = false;
static seq_t phi[PHI_SIZE];
static seq_t aarr[A_SIZE];

/* helper: build φ₁₅₇₇(x) and the first 1577 terms of its reciprocal series */
static void build_sequence(void)
{
    /* prime factors of 1577 */
    const idx_t p1 = 19;
    const idx_t p2 = 83;
    const idx_t n  = 1577;      /* same as N */

    /* numerator (x‑1)(xⁿ‑1) : degree n+1 = 1578 */
    const size_t num_len = n + 2;          /* indices 0 … n+1 */
    seq_t num[num_len];
    for (size_t i = 0; i < num_len; ++i) num[i] = 0;
    /* explicit multiplication: A(x)=x‑1, B(x)=xⁿ‑1 */
    num[0] += (seq_t)-1 * (seq_t)-1;   /* +1  at x⁰   */
    num[1] += (seq_t)1  * (seq_t)-1;   /* –1  at x¹   */
    num[n] += (seq_t)-1 * (seq_t)1;    /* –1  at xⁿ   */
    num[n+1] += (seq_t)1 * (seq_t)1;   /* +1  at xⁿ⁺¹ */

    /* denominator (xᵖ¹-1)(xᵖ²-1) : degree p1+p2 = 102 */
    const size_t den_len = p1 + p2 + 1;   /* indices 0 … 102 */
    seq_t den[den_len];
    for (size_t i = 0; i < den_len; ++i) den[i] = 0;
    den[0] = 1;                         /* +1  at x⁰   */
    den[p1] = (seq_t)-1;                 /* –1  at xᵖ¹   */
    den[p2] = (seq_t)-1;                 /* –1  at xᵖ²   */
    den[p1 + p2] = 1;                    /* +1  at xᵖ¹⁺ᵖ² */

    /* polynomial long division: num / den  (den is monic) */
    const size_t degN = n + 1;           /* 1578 */
    const size_t degD = p1 + p2;         /* 102  */
    const size_t degQ = degN - degD;     /* 1476 */

    seq_t tmp[num_len];
    for (size_t i = 0; i < num_len; ++i) tmp[i] = num[i];

    /* process from the highest degree downwards */
    for (ptrdiff_t i = degN; i >= (ptrdiff_t)degD; --i) {
        seq_t coef = tmp[i];
        if (coef != 0) {
            size_t shift = (size_t)(i - degD);
            phi[shift] = coef;          /* quotient coefficient */

            /* subtract coef * den shifted by 'shift' from tmp */
            for (size_t j = 0; j <= degD; ++j) {
                tmp[shift + j] -= coef * den[j];
            }
        }
    }

    /* the remainder is zero – quotient is φ₁₅₇₇(x) */

    /* build the reciprocal series via convolution with φ */
    aarr[0] = 1;                         /* a(0) = 1 */
    for (size_t k = 1; k < A_SIZE; ++k) {
        seq_t sum = 0;
        size_t upper = (k <= PHI_DEG) ? k : PHI_DEG;
        for (size_t i = 1; i <= upper; ++i) {
            sum += phi[i] * aarr[k - i];
        }
        aarr[k] = -sum;
    }

    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready) build_sequence();
    return aarr[(size_t)(n % N)];
}

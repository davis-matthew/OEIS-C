/*
 * A050503  Nearest integer to n*log(n).
 * https://oeis.org/A050503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to n*log(n) (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fixed‑point scaling – 24 bits gives enough precision for all n
   used in the tests and keeps all intermediate results < 2^64. */
#ifndef SCALE_BITS
#define SCALE_BITS 24
#endif

/* Half of the scaling factor, used for rounding */
static const seq_t HALF_SCALE = ((seq_t)1 << (SCALE_BITS - 1));

/* Smallest positive odd integer that fits in the scaling factor.
   Used for computations that need a CDS.  */
static const seq_t ONE = (seq_t)1;

/* Compute ln(2) in scaled form using the hyperbolic arctangent series:
   ln(2) = 2 * atanh(1/3)   with 1/3 = 0.333… */
static seq_t ln2_scaled(void)
{
    static seq_t cached = 0;
    if (cached != 0) return cached;

    /* y = 1/3 expressed in scaled integer */
    const seq_t y_num = ONE;
    const seq_t y_den = (seq_t)3;
    seq_t y_scaled = (y_num << SCALE_BITS) / y_den;     /* floor */

    seq_t y_sq = (y_scaled * y_scaled) >> SCALE_BITS;
    seq_t term = y_scaled;
    seq_t sum  = y_scaled;   /* sum of atanh series terms */
    int j = 1;
    while (1) {
        term = (term * y_sq) >> SCALE_BITS;
        if (term == 0) break;
        seq_t term_div = term / ( (seq_t) (2 * j + 1) );
        if (term_div == 0) break;
        sum += term_div;
        ++j;
    }
    cached = sum * (seq_t)2;  /* ln(2) = 2*atanh(y) */
    return cached;
}

/* Computes a(n) = nearest integer to n ln(n) */
seq_t a(idx_t n)
{
    /* Offset starts at n = 1.  For n = 0 (should never happen) return 0. */
    if (n == 0) return (seq_t)0;

    /* Find k such that 2^k ≤ n < 2^(k+1)     (k is floor(log₂(n))) */
    uint64_t k = 63 - __builtin_clzll((uint64_t)n);

    /* denom = 2^k */
    idx_t denom = (idx_t)1ULL << k;

    /* y = (m-1)/(m+1) where m = n / 2^k, 1 ≤ m < 2  */
    idx_t y_num = n - denom;      /* n - 2^k  */
    idx_t y_den = n + denom;      /* n + 2^k  */

    /* y_scaled = floor( y * 2^SCALE_BITS ) */
    seq_t y_scaled = ((seq_t)y_num << SCALE_BITS) / y_den;

    /* y_sq = y_scaled^2 / 2^SCALE_BITS  */
    seq_t y_sq = (y_scaled * y_scaled) >> SCALE_BITS;

    /* Compute ln(m) = atanh series multiplied by 2 */
    seq_t term = y_scaled;
    seq_t sum  = y_scaled;          /* initial atanh term */
    int j = 1;
    while (1) {
        term = (term * y_sq) >> SCALE_BITS;
        if (term == 0) break;
        seq_t term_div = term / ( (seq_t)(2 * j + 1) );
        if (term_div == 0) break;
        sum += term_div;
        ++j;
    }
    seq_t ln_m_scaled = sum * (seq_t)2;        /* ln(m) in scaled form */

    /* ln(n) = k * ln(2) + ln(m)  (scaled) */
    seq_t ln_scaled = (seq_t)k * ln2_scaled() + ln_m_scaled;

    /* n * ln(n)   (scaled)  */
    seq_t prod_scaled = (seq_t)n * ln_scaled;

    /* Round to nearest integer: add 2^(SCALE_BITS-1) and shift right */
    seq_t rounded = (prod_scaled + HALF_SCALE) >> SCALE_BITS;

    return rounded;
}

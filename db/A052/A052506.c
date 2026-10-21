/*
 * A052506  Expansion of e.g.f. exp(x*exp(x)-x).
 * https://oeis.org/A052506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..n} binom(n,k)·(n−k−1)^k (OEIS A052506)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation in seq_t.  seq_t is an unsigned big‑int type. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return 1;

    seq_t sum = 0;      /* running sum of positive terms */
    seq_t C   = 1;      /* current binomial coefficient binom(n, k) */

    /* k = 0 .. n-1: base = n-k-1 is non‑negative */
    for (idx_t k = 0; k < n; ++k) {
        seq_t base   = (seq_t)(n - k - 1);     /* base ≥ 0 for k < n */
        seq_t power  = pow_seq(base, k);
        sum += C * power;

        /* update C for next k:  binom(n, k+1) = binom(n, k) * (n-k)/(k+1) */
        C = C * (n - k) / (k + 1);
    }

    /* k = n term: base = -1, (−1)^n contributes ±1 */
    if ((n & 1U) == 0U)   /* even n */
        sum += 1;
    else                  /* odd n */
        sum -= 1;

    return sum;
}

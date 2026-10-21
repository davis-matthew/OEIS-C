/*
 * A091596  Expansion of x*(1-2*x^2)/(1-x-2*x^2)^2.
 * https://oeis.org/A091596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((3*n + 8)*2^n + (3*n - 8)*(-1)^n) / 27
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 25, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply by 2 repeatedly to compute 2^n in the type seq_t. */
static seq_t pow2(idx_t n)
{
    seq_t r = 1;
    for (idx_t i = 0; i < n; ++i)
        r = r * ((seq_t)2);
    return r;
}

seq_t a(idx_t n)
{
    /* (3*n + 8) */
    seq_t coeff = (seq_t)(3 * (uint64_t)n + 8);

    /* 2^n */
    seq_t pow_val = pow2(n);

    /* term1 = (3*n + 8) * 2^n */
    seq_t term1 = coeff * pow_val;

    /* Compute (3*n - 8) with signed arithmetic to handle small n */
    int64_t diff = (int64_t)3 * (int64_t)n - 8;

    /* Apply (-1)^n: if n is odd, negate diff */
    int64_t signed_diff = (n & 1ULL) ? -diff : diff;

    /* Start with term1, then add or subtract |signed_diff| */
    seq_t sum = term1;
    if (signed_diff >= 0)
        sum += (seq_t)signed_diff;
    else
        sum -= (seq_t)(-signed_diff);

    /* Divide by 27 – the division is always exact */
    return sum / (seq_t)27;
}

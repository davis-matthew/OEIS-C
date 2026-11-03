/*
 * A159321  L.g.f.: Sum_{n>=1} a(n)*x^n/n = Sum_{n>=1} (1 + C(2n-1,n-1)*x)^n *
 *          x^n/n.
 * https://oeis.org/A159321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{k=0}^{⌊n/2⌋} C(n-k,k) * C(2n-2k-1,n-k-1)^k / (n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the utility functions binom and pow_seq. */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Return 0 for n = 0 for safety. */
    if (n == 0)
        return 0;

    seq_t sum = 0;
    idx_t half = n / 2;                     /* ⌊n/2⌋ */

    for (idx_t k = 0; k <= half; ++k) {
        /* C(n-k, k) */
        seq_t c1 = binomial(n - k, k);

        /* C(2n-2k-1, n-k-1) */
        idx_t a_val = 2 * n - 2 * k - 1;
        idx_t b_val = n - k - 1;
        seq_t c2 = binomial(a_val, b_val);

        /* c2^k */
        seq_t c2_pow = ipow_i32(c2, (int)k);

        /* term = c1 * c2_pow * n / (n-k) */
        seq_t term = c1 * c2_pow;          /* multiplicative intermediate */
        term = term * (seq_t)n;            /* multiply by n            */
        term = term / (n - k);             /* divide by (n-k)          */

        sum += term;
    }

    return sum;
}

/*
 * A196457  E.g.f.: A(x) = Sum_{n>=0} exp((2^n + (-1)^n)*x) * (2^n + (-1)^n)^n
 *          * x^n/n!.
 * https://oeis.org/A196457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * (2^k + (-1)^k)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-01)
 * Helpers:   binomial, ipow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the binomial formula from the OEIS entry for A196457. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t exp_n   = (seq_t)n;               /* exponent n */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t pow2k = pow2(k);              /* 2^k */
        seq_t bk = (k % 2 == 0)
                   ? (pow2k + (seq_t)1)
                   : (pow2k - (seq_t)1);     /* 2^k + (-1)^k */

        seq_t val_pow = ipow(bk, exp_n); /* (2^k + (-1)^k)^n */
        seq_t bin     = binomial(n, k);        /* C(n,k) */

        result += bin * val_pow;            /* accumulate term */
    }

    return result;
}

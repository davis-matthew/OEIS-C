/*
 * A206156  a(n) = Sum_{k=0..n} binomial(n,k)^(2*k).
 * https://oeis.org/A206156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n,k)^(2*k) (OEIS A206156)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS sequence A206156.          */

/* Required utilities */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t c   = binomial(n, k);          /* C(n,k) */
        seq_t exp = (seq_t)k * 2ULL;         /* 2*k as seq_t */
        seq_t term = ipow(c, exp);        /* C(n,k)^(2*k) */
        sum += term;                         /* accumulate */
    }
    return sum;
}

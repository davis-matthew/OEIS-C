/*
 * A206156  a(n) = Sum_{k=0..n} binomial(n,k)^(2*k).
 * https://oeis.org/A206156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n,k)^(2*k) (OEIS A206156)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS sequence A206156.          */

/* Required utilities */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/big_pow.h"    /* seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t c   = binomial(n, k);          /* C(n,k) */
        seq_t exp = (seq_t)k * 2ULL;         /* 2*k as seq_t */
        seq_t term = big_pow(c, exp);        /* C(n,k)^(2*k) */
        sum += term;                         /* accumulate */
    }
    return sum;
}

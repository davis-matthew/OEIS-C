/*
 * A225996  Number of pairs of orthogonal (-4,5) vectors of length 9*n.
 * https://oeis.org/A225996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Σ_{t=0}^{n} binomial(9n,5t) *
 *            binomial(9n-5t,4(n-t)) * 2^{5n-t}
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   binom, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts unordered pairs of orthogonal vectors of length 9*n with entries -4 or 5. */

#include "../utils/binom.h"   /* provides binom(idx_t a, idx_t b) */
#include "../utils/pow_int.h" /* provides pow_int(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* n >= 1; intermediate values use idx_t for loop indices and seq_t for arithmetic */
    idx_t L = 9 * n;                /* length of vectors */
    seq_t sum = 0;

    for (idx_t t = 0; t <= n; ++t) {
        /* p = 5t, q = 4(n-t), r = 5n-t */
        seq_t term = binom(L, 5 * t);                  /* binomial(9n,5t)          */
        term *= binom(L - 5 * t, 4 * (n - t));         /* binomial(9n-5t,4(n-t))  */
        term *= pow_int((seq_t)2, 5 * n - t);           /* 2^{5n-t}                */
        sum += term;
    }

    return sum / 2;  /* unordered pairs */
}

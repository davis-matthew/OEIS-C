/*
 * A009017  Expansion of e.g.f. cos(x*exp(x)).
 * https://oeis.org/A009017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} binomial(n,2k) * (2k)^(n-2k) * (-1)^k
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 23, i128 n <= 39,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation relies on the helper functions
   binomial(idx_t n, idx_t k) and big_pow(seq_t base, seq_t exp) from
   the utils library. */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t twok = 2 * k;
        seq_t bin = binomial(n, twok);
        /* Compute (2k)^(n-2k). */
        seq_t power = big_pow((seq_t)twok, (seq_t)(n - twok));
        seq_t term = bin * power;
        if (k & 1) term = -term;
        sum += term;
    }
    return sum;
}

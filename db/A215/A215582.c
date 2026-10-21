/*
 * A215582  The number of proper mergings of two n-antichains.
 * https://oeis.org/A215582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{i+j+k=n} n!/(i!j!k!) * (-1)^k * (2^i + 2^j - 1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   binom, pow2, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"   /* binom(idx_t, idx_t) -> seq_t */
#include "../utils/pow2.h"    /* pow2(idx_t) -> seq_t */
#include "../utils/seq_pow.h" /* seq_pow(seq_t, idx_t) -> seq_t */

seq_t a(idx_t n) {
    seq_t sum = 0;                          /* running total */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t pow2_i = pow2(i);              /* 2^i */
        seq_t mult_i = binom(n, i);          /* multinomial part n!/(i!*(n-i)!) */
        for (idx_t j = 0; j <= n - i; ++j) {
            idx_t k = n - i - j;             /* remaining part */
            seq_t bin_j   = binom(n - i, j); /* (n-i)!/(j!k!) */
            seq_t mult    = mult_i * bin_j;  /* n!/(i!j!k!) */

            seq_t pow2_j = pow2(j);          /* 2^j */
            seq_t base   = pow2_i + pow2_j - 1; /* 2^i + 2^j - 1 */
            seq_t power  = seq_pow(base, n);   /* (2^i+2^j-1)^n */

            seq_t term = mult * power;        /* multinomial * power */
            if (k & 1) term = -term;          /* (-1)^k factor */

            sum += term;                     /* accumulate */
        }
    }
    return sum;
}

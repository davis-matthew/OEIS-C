/*
 * A109388  Maximum number of pairwise incomparable subcubes of the discrete
 *          n-cube. Largest antichain in partial ordering {0,1,*}^n where 0
 *          and 1 are less than *. Maximum number of implicants in an
 *          irredundant disjunctive normal form for n Boolean variables.
 * https://oeis.org/A109388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n, floor(n/3)) * 2^(n - floor(n/3))
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t k = n / 3;                    /* floor(n/3) */
    seq_t comb = binomial(n, k);        /* C(n, k) */
    seq_t pow  = ((seq_t)1) << (n - k); /* 2^(n-k) */
    return comb * pow;
}

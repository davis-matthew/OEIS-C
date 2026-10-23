/*
 * A032095  Number of reversible strings with n-1 black beads and n-1 white
 *          beads. String is not palindromic.
 * https://oeis.org/A032095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, if n = 1; otherwise: if n is even: a(n) = binom(2n-2,
 *            n-1) / 2 if n is odd : a(n) = ( binom(2n-2, n-1) - binom(n-1,
 *            (n-1)/2) ) / 2 where binom(a,b) is the binomial coefficient
 *            C(a,b)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    /* Compute C(2n-2, n-1) */
    idx_t top   = (idx_t)(2 * n - 2);   /* 2n-2 fits in idx_t for usual limits */
    idx_t bottom = n - 1;               /* n-1 */
    seq_t central = binomial(top, bottom);

    if ((n & 1ull) == 0) {              /* n even */
        return central / 2;
    } else {                            /* n odd */
        /* Compute C(n-1, (n-1)/2) */
        idx_t half = (n - 1) / 2;
        seq_t middle = binomial(n - 1, half);
        return (central - middle) / 2;
    }
}

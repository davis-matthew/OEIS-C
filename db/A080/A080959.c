/*
 * A080959  Square array of coefficients of binomial polynomials, read by
 *          antidiagonals.
 * https://oeis.org/A080959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is obtained by reading the anti‑diagonals of the square
 *            array A(i,j) where A(i,j) = j! * Σ_{t=1..j} (-1)^(t+1) *
 *            binomial(i+t, t) / t. The sequence starts with a(1)=1
 * Range:     i8 n <= 19, i16 n <= 35, i32 n <= 90, i64 n <= 230,
 *            i128 n <= 594, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

static inline seq_t factorial_seq(idx_t k) {
    seq_t f = 1;
    for (idx_t i = 2; i <= k; ++i) f *= (seq_t)i;
    return f;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n=0, but safe to return 0 */
    /* Find minimal s such that s*(s+1)/2 >= n */
    idx_t low = 1, high = 2;
    while (((unsigned __int128)high * (high + 1)) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 cum = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (cum >= n) high = mid;
        else low = mid + 1;
    }
    idx_t s = low;                     /* diagonal index (s = i + j)      */
    idx_t prev = s * (s - 1) / 2;       /* cumulative terms before this diagonal */
    idx_t p = n - prev - 1;             /* position within the diagonal (0‑based) */
    idx_t i = s - 1 - p;                /* row index of A(i,j) (i >= 0)      */
    idx_t j = s - i;                    /* column index (j >= 1)             */

    seq_t kfact = factorial_seq(j);
    seq_t sum = 0;
    for (idx_t t = 1; t <= j; ++t) {
        seq_t bin = binom((idx_t)(i + t), t);          /* binomial(i+t, t) */
        seq_t term = (kfact / (seq_t)t) * bin;         /* j! / t * binom(...) */
        if ((t & 1) == 0) term = -term;                /* (-1)^(t+1) sign */
        sum += term;
    }
    return sum;
}

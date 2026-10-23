/*
 * A157125  A transform of the Catalan numbers.
 * https://oeis.org/A157125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^C(n-k+1,2) * binomial(floor((n-k)/2),k)
 *            * Catalan(k)
 * Range:     i8 n <= 14, i16 n <= 26, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/catalan.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t t = n - k;
        idx_t half = t >> 1;
        if (k > half) continue;            /* binomial will be zero otherwise */

        /* parity of C(t+1,2) = t*(t+1)/2  (mod 2) */
        idx_t prod = (t + 1) * t;
        idx_t parity = (prod >> 1) & 1;      /* 0 = even, 1 = odd */

        seq_t bin = binom(half, k);
        seq_t cat = catalan(k);
        seq_t term = bin * cat;
        if (parity) term = -term;           /* multiply by (-1) if odd */

        sum += term;
    }
    return sum;
}

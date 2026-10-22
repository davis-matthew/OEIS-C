/*
 * A003272  a(n) = ceiling((-4n)/Bernoulli(2n)).
 * https://oeis.org/A003272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ceiling(-4n/Bernoulli(2n)), a(0) = 0; for n >= 10,
 *            |B(2n)| > 4n, so a(n) = 1 for even n and 0 for odd n
 * Range:     i8 n <= 1, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   a3272_bern, a3272_g, a3272_norm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a3272_bern.h"
#include "../utils/a3272_g.h"
#include "../utils/a3272_norm.h"

seq_t a(idx_t n) {
    long long bnum, bden, p, q, r;
    if (n == 0) return 0;
    if (n >= 10) return (n & 1) ? (seq_t)0 : (seq_t)1;
    a3272_bern(2 * (int)n, &bnum, &bden);
    /* value = -4n / (bnum/bden) = -4n*bden / bnum */
    p = -4LL * (long long)n * bden;
    q = bnum;
    if (q < 0) {
        p = -p;
        q = -q;
    }
    if (p >= 0)
        r = (p + q - 1) / q;
    else
        r = p / q;
    return (seq_t)r;
}

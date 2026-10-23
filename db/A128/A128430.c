/*
 * A128430  Triangle read by rows: A054524 * A000012.
 * https://oeis.org/A128430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = \sum_{d\mid r,\ d\ge c} \mu(d) * where r is the row
 *            number and c is the column number * of the index n in the
 *            triangle. * r = ceil((sqrt(8n+1)-1)/2), c = n - r(r-1)/2
 * Range:     i8 n <= 55, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static seq_t mobius(idx_t m)
{
    if (m == 1) return 1;
    seq_t mu = 1;
    idx_t t = m;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int cnt = 0;
            while (t % p == 0) {
                t /= p;
                ++cnt;
                if (cnt > 1) return 0;      /* square factor */
            }
            mu = -mu;                         /* multiply by -1 for each distinct prime factor */
        }
    }
    if (t > 1) mu = -mu;                      /* remaining prime factor */
    return mu;
}

seq_t a(idx_t n)
{
    /* compute row r */
    idx_t r = (idx_t)((sqrtl((long double)8 * n + 1.0L) - 1.0L) / 2.0L);
    if (r * (r + 1) / 2 < n) ++r;          /* ensure r is large enough */

    /* column in the row */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t c = n - prev_tri;

    /* sum over divisors of r that are >= c */
    seq_t sum = 0;
    idx_t limit = (idx_t)sqrtl((long double)r);
    for (idx_t d = 1; d <= limit; ++d) {
        if (r % d == 0) {
            idx_t d1 = d;
            if (d1 >= c) sum += mobius(d1);
            idx_t d2 = r / d;
            if (d2 != d1 && d2 >= c) sum += mobius(d2);
        }
    }
    return sum;
}

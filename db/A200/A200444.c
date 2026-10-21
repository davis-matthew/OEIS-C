/*
 * A200444  Number of 0..n arrays x(0..7) of 8 elements with zero 3rd
 *          differences.
 * https://oeis.org/A200444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 142, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A200444: Number of 8‑element arrays x(0..7) with entries in {0,…,n}
 * that have zero third finite differences (i.e. the entries lie on a
 * quadratic polynomial).
 *
 * Implementation: Enumerate p0 = x(0), p1 = x(1), p2 = x(2) in [0,n].
 * For any such triple the values of the quadratic at k=0,1,2 determine
 * an integer‑valued quadratic polynomial of the form
 *
 *     p(k) = C + B*k + A*k*(k-1)/2
 *
 * where
 *     C = p0
 *     B = p1 - p0
 *     A = p2 - 2*p1 + p0
 *
 * The remaining values at k=3..7 must also lie in [0,n], giving linear
 * constraints on the unknown p2.  For each fixed pair (p0,p1) the set of
 * admissible p2 is a contiguous integer interval; its length is added to
 * the count.  The algorithm runs in O(n²) time with a very small
 * constant factor (at most five inequalities per pair).
 */

#include <stdint.h>

/* Signed 64‑bit helper for division with rounding.  Divisor is >0. */
static inline int64_t ceil_div(int64_t a, int64_t b) {
    int64_t q = a / b;
    int64_t r = a % b;
    if (r != 0 && ((a > 0) == (b > 0)))
        ++q;
    return q;
}

static inline int64_t floor_div(int64_t a, int64_t b) {
    int64_t q = a / b;
    int64_t r = a % b;
    if (r != 0 && ((a < 0) != (b < 0)))
        --q;
    return q;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* offset is 1; values below 1 are undefined */

    /* Pre‑computed binomial coefficient k(k-1)/2 for k = 3..7 */
    const int64_t Ck[5] = {3, 6, 10, 15, 21};

    seq_t total = 0;

    for (idx_t p0 = 0; p0 <= n; ++p0) {
        for (idx_t p1 = 0; p1 <= n; ++p1) {
            int64_t d     = (int64_t)p1 - (int64_t)p0;          /* B = d          */
            int64_t lower = 0;                                 /* lower bound on p2 */
            int64_t upper = (int64_t)n;                         /* upper bound on p2 */
            int      ok   = 1;

            /* Derive bounds on p2 from k = 3..7 */
            for (int i = 0; i < 5; ++i) {
                int k   = i + 3;
                int64_t C = Ck[i];

                int64_t base = (int64_t)p0 + k * d - C * ((int64_t)2 * p1 - (int64_t)p0);

                int64_t l = ceil_div(-base, C);
                int64_t u = floor_div((int64_t)n - base, C);

                if (l > lower)
                    lower = l;
                if (u < upper)
                    upper = u;
                if (lower > upper) {   /* intersection empty */
                    ok = 0;
                    break;
                }
            }

            if (ok && lower <= upper) {
                seq_t count = (seq_t)(upper - lower + 1);
                total += count;
            }
        }
    }

    return total;
}

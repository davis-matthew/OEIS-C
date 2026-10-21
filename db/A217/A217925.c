/*
 * A217925  G.f. A(x) satisfies A(x) = 1 + x*A(x)*A(x^2)^2.
 * https://oeis.org/A217925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{j=0..⌊(n-1)/2⌋} a(n-1-2*j) * Σ_{k=0..j}
 *            a(k)*a(j-k) (n ≥ 1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Helpers:   ensure_capacity_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_capacity_seq_t.h"

seq_t a(idx_t n)
{
    /* static storage for memoisation across calls */
    static seq_t *a_seq = NULL;      /* a[0 ..]                */
    static seq_t *s_seq = NULL;      /* s[0 ..]                  */
    static size_t a_cap = 0;         /* capacity of a_seq       */
    static size_t s_cap = 0;         /* capacity of s_seq       */
    static idx_t  a_len = 0;         /* number of a values computed (next index to fill) */

    /* allocate or expand arrays to hold up to n inclusive */
    size_t need = (size_t)n + 1;
    ensure_capacity_seq_t(&a_seq, &a_cap, need);
    ensure_capacity_seq_t(&s_seq, &s_cap, need);

    /* initialise first term if not yet done */
    if (a_len == 0) {
        a_seq[0] = (seq_t)1;
        s_seq[0] = (seq_t)1;      /* s[0] = a[0]*a[0] */
        a_len = 1;
    }

    /* if we already have the requested term, return it */
    if (n < a_len)
        return a_seq[n];

    /* compute missing terms up to n */
    for (idx_t m = a_len; m <= n; ++m) {
        /* compute a[m] for m > 0 */
        if (m > 0) {
            seq_t sum = 0;
            idx_t max_j = (m - 1) / 2;   /* floor((m-1)/2) */
            for (idx_t j = 0; j <= max_j; ++j) {
                sum += a_seq[m - 1 - 2 * j] * s_seq[j];
            }
            a_seq[m] = sum;
        }

        /* compute s[m] for m > 0 */
        if (m > 0) {
            seq_t sm = 0;
            /* term for k = 0 and k = m (both equal a[0]*a[m]) */
            seq_t term = a_seq[0] * a_seq[m];
            sm = term + term;          /* double it */

            /* add contributions for 1 <= k <= (m-1)/2 */
            for (idx_t k = 1; k <= (m - 1) / 2; ++k) {
                seq_t prod = a_seq[k] * a_seq[m - k];
                sm += prod + prod;     /* double */
            }
            /* if m is even, add middle term once */
            if ((m & 1) == 0) {
                seq_t mid = a_seq[m / 2];
                sm += mid * mid;
            }
            s_seq[m] = sm;
        }
    }

    a_len = n + 1;   /* update length of computed a values */
    return a_seq[n];
}

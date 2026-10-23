/*
 * A123574  The Kruskal-Macaulay function K_5(n).
 * https://oeis.org/A123574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = K_5(n) where n is expressed uniquely as n = C(b_5,5) +
 *            C(b_4,4) + C(b_3,3) + C(b_2,2) + C(b_1,1) (strictly decreasing
 *            b_5 > b_4 > b_3 > b_2 > b_1 >= 1), then a(n) = C(b_5,4) +
 *            C(b_4,3) + C(b_3,2) + C(b_2,1) + C(b_1,0)
 * Range:     u8 n <= 55, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <limits.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t rem = n;          /* remaining value to decompose */
    seq_t result = 0;       /* accumulated Kruskal-Macaulay value */
    idx_t prev_b = ULLONG_MAX; /* b_{i+1} (next larger index) */

    for (int i = 5; i >= 1; --i) {
        if (rem == 0)
            break;

        /* Special handling for i = 1 to avoid a long linear scan. */
        if (i == 1) {
            idx_t chosen = rem;
            if (prev_b != ULLONG_MAX && chosen >= prev_b)
                chosen = prev_b - 1;
            if (chosen == 0)
                break;
            rem -= chosen;
            result += 1;              /* C(chosen,0) = 1 */
            prev_b = chosen;
            break;                    /* remainder will be zero */
        }

        /* Find the largest b < prev_b with C(b,i) <= rem. */
        idx_t low = i;
        idx_t chosen = 0;
        for (idx_t b = low; ; ++b) {
            if (prev_b != ULLONG_MAX && b >= prev_b)   /* enforce strict decrease */
                break;
            seq_t val = binomial(b, (idx_t)i);
            if (val > (seq_t)rem)
                break;
            chosen = b;  /* keep the largest valid candidate seen so far */
        }

        if (chosen == 0)   /* no suitable term for this i (not expected) */
            continue;

        seq_t val = binomial(chosen, (idx_t)i);
        rem -= (idx_t)val;                  /* rem >= 0 */
        result += binomial(chosen, (idx_t)(i - 1));
        prev_b = chosen;
    }

    return result;
}

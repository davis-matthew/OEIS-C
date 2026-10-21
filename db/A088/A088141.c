/*
 * A088141  a(n) = the largest k such that, if k samples are taken from a
 *          group of n items, with replacement, a duplication is unlikely
 *          (p<1/2).
 * https://oeis.org/A088141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k such that (n)_k / n^k > 1/2 (where (n)_k = n *
 *            (n-1) * ... * (n-k+1)). This is A033810(n) - 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* External integer types are supplied by the build. */
seq_t a(idx_t n)
{
    /* Handle the trivial case for consistency */
    if (n == 0) return (seq_t)0;

    double p = 1.0;   /* current probability of no duplicate */
    idx_t k = 0;      /* current sample count */

    /* Iterate while the probability stays above 1/2. */
    while (true) {
        /* Compute next probability: multiply by (n - k) / n */
        double factor = (double)(n - k) / (double)n;
        p *= factor;

        /* If the probability of a duplicate is still unlikely
           (i.e., probability of no duplicate > 1/2), we can
           safely take another sample. */
        if (p > 0.5) {
            k++;
        } else {
            break;
        }
    }

    return (seq_t)k;
}

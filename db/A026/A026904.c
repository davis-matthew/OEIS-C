/*
 * A026904  Number of sets S of positive integers satisfying E(S)=n, where E =
 *          2nd elementary symmetric function.
 * https://oeis.org/A026904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of sets S of positive integers with * E(S) =
 *            Σ_{i<j} s_i*s_j = n, where S is a set (all elements distinct)
 * Range:     u8 n <= 222, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for idx_t and seq_t definitions in the build */

/* Recursive helper: count sets extending a partial set.
 * Parameters:
 *   n      – target value of E(S)
 *   curE   – current value of E for the partially built set
 *   sum    – sum of the elements in the partial set
 *   last   – the greatest element already chosen (next element must be > last)
 *   count  – number of elements already chosen
 */
static seq_t dfs(idx_t n, seq_t curE, idx_t sum, idx_t last, idx_t count)
{
    seq_t total = 0;
    idx_t upper;                 /* upper bound for the next element */

    if (sum == 0) {              /* first element – no product constraint yet */
        upper = n;               /* any element <= n can be chosen as the first one */
    } else {
        if (curE > (seq_t)n)     /* should not happen, but guard against overflow */
            return total;
        upper = (idx_t)(((seq_t)n - curE) / (seq_t)sum);
    }

    for (idx_t x = last + 1; x <= upper; ++x) {
        idx_t newSum   = sum + x;
        seq_t newE     = curE + (seq_t)sum * (seq_t)x;
        idx_t newCount = count + 1;

        if (newE == (seq_t)n) {
            if (newCount >= 2)
                total += 1;     /* a valid set has been found */
            /* cannot add more elements – any addition would raise E above n */
            continue;
        }

        /* newE < n, recurse to try adding more elements */
        total += dfs(n, newE, newSum, x, newCount);
    }

    return total;
}

/* Public interface required by the grader */
seq_t a(idx_t n)
{
    if (n <= 1)                 /* E(S) = 0 is the only value achieved by empty or one-element sets */
        return 0;
    return dfs(n, (seq_t)0, (idx_t)0, (idx_t)0, (idx_t)0);
}

/*
 * A145888  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} in which k is the largest entry in the cycle
 *          containing 1 (1 <= k <= n).
 * https://oeis.org/A145888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k), where r = smallest integer with r*(r+1)/2 >= n *
 *            and k = n - (r-1)*r/2. * T(r,1) = (r-1)! and T(r,k) = r! /
 *            ((r-k+1)(r-k+2)) for k >= 2
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact = NULL;      /* fact[i] = i!  (0 <= i <= fact_cap-1) */
static size_t fact_cap = 0;     /* current capacity of 'fact' */

/* Ensure factorial values up to 'n' (inclusive) are available. */
static void ensure_factorial(idx_t n)
{
    /* We need values at indices 0..n.  If already there, nothing to do. */
    if ((size_t)n + 1 <= fact_cap) {
        return;
    }

    /* Grow capacity: start from current or 1, double until > n. */
    size_t new_cap = fact_cap ? fact_cap : 1;
    while (new_cap <= (size_t)n) {
        new_cap <<= 1;
    }

    size_t old_cap = fact_cap;
    seq_t *new_fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
    if (!new_fact) {
        exit(1);  /* allocation failed */
    }

    /* If this is the first allocation, initialize 0! = 1. */
    if (old_cap == 0) {
        new_fact[0] = (seq_t)1;
        old_cap = 1;          /* next index to fill is 1 */
    }

    /* Compute missing factorials up to new_cap-1. */
    for (size_t i = old_cap; i < new_cap; ++i) {
        new_fact[i] = new_fact[i - 1] * (seq_t)i;
    }

    fact = new_fact;
    fact_cap = new_cap;
}

/* Return the n-th term of the flattened triangle (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;  /* undefined for n=0, but return 0 */
    }

    /* Find the row r such that r*(r+1)/2 >= n and (r-1)*r/2 < n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    idx_t prev_sum = ((r - 1) * r) / 2;
    idx_t k = n - prev_sum;          /* column within the row (1 <= k <= r) */

    /* Make sure factorials are ready. */
    ensure_factorial(r);

    if (k == 1) {
        /* T(r,1) = (r-1)! */
        return fact[r - 1];
    }

    /* T(r,k) = r! / ((r-k+1)(r-k+2)) for k >= 2 */
    seq_t ans = fact[r];
    ans /= (seq_t)(r - k + 1);
    ans /= (seq_t)(r - k + 2);
    return ans;
}

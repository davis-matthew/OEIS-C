/*
 * A107435  Triangle T(n,k), 1<=k<=n, read by rows: T(n,k) = length of
 *          Euclidean algorithm starting with n and k.
 * https://oeis.org/A107435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = length of Euclidean algorithm for the pair (r, k), where
 *            r is the unique integer such that r(r‑1)/2 < n ≤ r(r+1)/2 and k
 *            = n – r(r‑1)/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9031 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Return the number of divisions in the Euclidean algorithm on (n,k),
   with n >= k >= 1. */
static seq_t euclidean_steps(idx_t n, idx_t k)
{
    seq_t a = (seq_t)n;
    seq_t b = (seq_t)k;
    seq_t steps = 0;

    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
        ++steps;
    }
    return steps;
}

/* Compute the minimal r with r*(r+1)/2 >= n.  n >= 1. */
static idx_t find_row(idx_t n)
{
    idx_t low  = 1;
    idx_t high = 1;

    /* Increase high until it satisfies high*(high+1)/2 >= n */
    while ((high * (high + 1)) / 2 < n)
        high <<= 1;

    /* Binary search for the smallest high that satisfies the inequality. */
    while (low < high) {
        idx_t mid = (low + high) / 2;
        if ((mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, so n>=1 always in practice */
        return 0;

    /* Find the row r and column k corresponding to a(n). */
    idx_t r = find_row(n);
    idx_t prev = (r - 1) * r / 2;   /* number of terms before row r */
    idx_t k = n - prev;             /* 1 <= k <= r */

    return euclidean_steps(r, k);
}

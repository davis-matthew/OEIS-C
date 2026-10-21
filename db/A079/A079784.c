/*
 * A079784  Triangle read by rows in which the n-th row contains the smallest
 *          set of n consecutive numbers such that the r-th number in the n-th
 *          row is divisible by n-r+1. The first term of the n-th row must
 *          exceed n.
 * https://oeis.org/A079784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = first element of row r + column offset, where row r
 *            satisfies r(r+1)/2 >= n and the r‑th row starts at s = t *
 *            lcm(1..r) – r, with t = floor(2r / lcm(1..r)) + 1
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by constructing a triangular array.
   The n‑th row contains the smallest set of n consecutive integers
   such that the k‑th element (1‑based) of this row is divisible by
   (n – k + 1). The elements of the triangle are read by rows,
   producing the integer sequence a(1), a(2), … . */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/int_sqrt.h"   /* int_sqrt(idx_t n) -> floor(sqrt(n)) */

/* Cache of lcm(1..i) for i up to the largest requested row. */
static seq_t *lcm_cache = NULL;
static idx_t lcm_cache_len = 0;   /* number of valid entries (first index 0) */

/* Ensure that lcm_cache contains lcm(1..n) for all indices up to n. */
static void ensure_lcm_upto(idx_t n)
{
    if (n + 1 <= lcm_cache_len)
        return;    /* already enough */

    /* Grow cache to size n+1. */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(lcm_cache, new_len * sizeof(seq_t));
    if (!tmp)
        exit(1);   /* out of memory */
    lcm_cache = tmp;

    if (lcm_cache_len == 0) {
        /* convention: lcm(1..0) = 1 */
        lcm_cache[0] = (seq_t)1;
        lcm_cache_len = 1;
    }

    /* Compute missing entries. */
    for (idx_t k = lcm_cache_len; k <= n; ++k) {
        seq_t prev = lcm_cache[k - 1];
        seq_t g = gcd(prev, (seq_t)k);
        /* lcm(prev, k) = prev / g * k */
        seq_t val = prev / g * (seq_t)k;
        lcm_cache[k] = val;
    }
    lcm_cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* sequence is defined for n >= 1 */

    /* Find the row r such that r(r+1)/2 >= n. */
    idx_t disc   = 1 + 8 * n;                /* 1 + 8*n */
    idx_t sqrt_disc = int_sqrt(disc);        /* floor(sqrt(disc)) */
    idx_t r = (sqrt_disc - 1) / 2;
    if ((r * (r + 1)) / 2 < n)
        ++r;   /* adjust to satisfy the inequality */

    /* Column within the row (1‑based). */
    idx_t prev_sum = (r * (r - 1)) / 2;      /* sum of lengths of previous rows */
    idx_t c = n - prev_sum;                  /* 1 <= c <= r */

    /* Compute the start of row r. */
    ensure_lcm_upto(r);
    seq_t L = lcm_cache[r];

    /* t = floor(2r / L) + 1, computed in seq_t to avoid overflow. */
    seq_t t = ((seq_t)(2 * r) / L) + 1;

    seq_t first = L * t - (seq_t)r;          /* start of the r‑th row */

    return first + (seq_t)(c - 1);           /* nth element */
}

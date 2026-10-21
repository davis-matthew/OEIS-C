/*
 * A071138  CONTINUANT transform of {sigma(n)}, 1, 3, 4, 7, 6, 12, ...
 *          (A000203).
 * https://oeis.org/A071138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = continuant of the first n values of σ(n) where σ(n) = sum
 *            of divisors of n. Recurrence: a(1) = σ(1) a(2) = σ(2)·a(1) + 1
 *            a(n) = σ(n)·a(n−1) + a(n−2) (n ≥ 3) σ(0) is never used; a(0) is
 *            defined as 1 only for the recurrence. The implementation works
 *            for any unsigned integer type seq_t up to...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define MAX_N 10000  /* practical upper bound requested in the problem */

static seq_t sigma_arr[MAX_N + 1];   /* σ values, 1‑based; sigma_arr[0] unused */
static seq_t a_arr[MAX_N + 1];       /* continuant values, 0‑based; a_arr[0] = 1 */
static idx_t sigma_max = 0;          /* highest index computed for σ  */
static idx_t a_max     = 0;          /* highest index computed for a  */

/* Recompute σ up to n (inclusive).  The whole array is rebuilt each time
   to keep the code simple and to avoid dynamic allocations of non‑trivial
   types.  For n ≤ MAX_N this runs in O(n log n) and is inexpensive for
   the required limits. */
static void compute_sigma_upto(idx_t n)
{
    if (n <= sigma_max)
        return;
    if (n > MAX_N)  /* safety guard – the implementation only supports up to MAX_N */
        return;

    /* zero out needed entries */
    for (idx_t i = 1; i <= n; ++i)
        sigma_arr[i] = (seq_t)0;

    /* accumulate divisors */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i)
            sigma_arr[j] += (seq_t)i;
    }
    sigma_max = n;
}

/* Extend the continuant sequence up to n (inclusive).  Assumes
   σ has already been computed up to n.  a_arr[0] is set to 1 on first
   use. */
static void compute_a_upto(idx_t n)
{
    if (n <= a_max)
        return;
    if (n > MAX_N)  /* safety guard */
        return;

    if (a_max == 0) {
        a_arr[0] = (seq_t)1;  /* a(0) = 1, not part of OEIS but needed for recurrence */
    }

    for (idx_t i = a_max + 1; i <= n; ++i) {
        if (i == 1) {
            a_arr[1] = sigma_arr[1];
        } else {
            a_arr[i] = sigma_arr[i] * a_arr[i - 1] + a_arr[i - 2];
        }
    }
    a_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;  /* defined only for compatibility */

    if (n > MAX_N)        /* safety: values beyond the supported range are undefined */
        return (seq_t)0;

    compute_sigma_upto(n);
    compute_a_upto(n);
    return a_arr[n];
}

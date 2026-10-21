/*
 * A054028  a(n) = first k such that 2^k >= k^n, (for n >= 2, k >= 2).
 * https://oeis.org/A054028
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by binary search on the inequality
   k ≥ n·log₂(k).  The smallest k ≥ 2 satisfying this
   condition is a(n).  This is equivalent to 2^k ≥ k^n. */
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

/* local helper – true iff k satisfies k ≥ n·log₂(k)   */
static inline bool satisfies(idx_t k, idx_t n)
{
    /* k≥2 for the search; k==1 never satisfies for n≥2 */
    if (k < 2) return false;

    /* use long double for sufficient precision              */
    long double logk   = log2l((long double)k);
    long double rhs    = (long double)n * logk;

    /* compare k with n·log₂(k).  A tiny epsilon protects
       against rounding errors for non‑power‑of‑two k's.    */
    return (long double)k >= rhs - 1e-12L;
}

seq_t a(idx_t n)
{
    /* undefined for n < 2 – return 0 as a safe sentinel   */
    if (n < 2) return (seq_t)0;

    /* find an upper bound where the inequality holds      */
    idx_t low  = 2;
    idx_t high = 2;
    while (!satisfies(high, n)) {
        /* guard against overflow – high will get huge enough soon */
        if (high > (UINT64_MAX >> 1))
            break;
        high <<= 1;
    }

    /* binary search the minimal k that satisfies the inequality   */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (satisfies(mid, n))
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}

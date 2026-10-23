/*
 * A038567  Denominators in canonical bijection from positive integers to
 *          positive rationals <= 1.
 * https://oeis.org/A038567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, core, easy, nice
 * Formula:   a(n) = smallest positive integer d such that Σ_{k=1}^d φ(k) > n,
 *            where φ is Euler's totient function. This is the denominator of
 *            the n‑th fraction (0‑based) in the canonical bijection between
 *            ℕ⁺ and reduced rationals ≤1
 * Range:     u8 n <= 688, u16 n <= 9835, u32 n <= 9835, u64 n <= 9835,
 *            u128 n <= 9835, big any n
 * Helpers:   phi_brute
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/phi_brute.h"

/* Recasting macro works for both C and C++ */
#if defined(__cplusplus)
#define RECAST(ptr, type) reinterpret_cast<type>(ptr)
#else
#define RECAST(ptr, type) ((type)(ptr))
#endif

/* Forward declaration of φ via brute force */
static seq_t phi_brute(idx_t m);

/* Cumulative sums Σ_{k=1}^d φ(k) for successive denominators d.
   cum_sums[i]  == Σ_{k=1}^{i+1} φ(k).  */
static seq_t *cum_sums = NULL;
static size_t cum_size = 0;
static size_t cum_cap  = 0;

/* Expand cumulative sums until the last entry exceeds target. */
static void ensure_cum(seq_t target)
{
    if (cum_size == 0) {
        cum_cap = 16;
        cum_sums = RECAST(malloc(cum_cap * sizeof(seq_t)), seq_t *);
        if (!cum_sums) abort();
        cum_size = 0;
    }

    /* Add new terms as necessary */
    while (cum_size == 0 || cum_sums[cum_size-1] <= target) {
        idx_t d = (idx_t)(cum_size + 1);        /* next denominator */
        seq_t phi = phi_brute(d);
        seq_t sum = cum_size == 0 ? phi : cum_sums[cum_size-1] + phi;

        /* Grow the array if needed */
        if (cum_size >= cum_cap) {
            size_t new_cap = cum_cap * 2;
            seq_t *tmp = RECAST(realloc(cum_sums, new_cap * sizeof(seq_t)), seq_t *);
            if (!tmp) abort();
            cum_sums = tmp;
            cum_cap = new_cap;
        }

        cum_sums[cum_size++] = sum;
    }
}

/* Binary search for the first index i with cum_sums[i] > target. */
static size_t lower_bound(seq_t target)
{
    size_t lo = 0, hi = cum_size;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (cum_sums[mid] > target)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;                       /* lo == cum_size means no such entry */
}

/* Main access function required by the framework. */
seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;          /* convert to seq_t for comparisons */
    ensure_cum(target);               /* make sure we have a sum > target */
    size_t idx = lower_bound(target); /* index of first sum > target   */
    return (seq_t)(idx + 1);          /* denominator = index+1           */
}

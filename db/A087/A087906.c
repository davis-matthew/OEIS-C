/*
 * A087906  a(n) = Sum_{d|n} (n-1)!/(d-1)!.
 * https://oeis.org/A087906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A087906
 *  a(n) = Sum_{d|n} (n-1)!/(d-1)!
 *  The implementation computes the factorial (n‑1)! once per call
 *  and then divides it by (d‑1)! for each divisor d of n.
 *  The divisor list is generated, sorted and processed in
 *  ascending order; the factorial values are built incrementally
 *  as needed.  This avoids storing the very large factorial array,
 *  keeps the memory usage low and runs in O(n + τ(n)) time,
 *  where τ(n) is the number of divisors of n (≤ 200 for n ≤ 10000).
 */

#include <stdlib.h>
#include <stdbool.h>

/* Integer square‑root for unsigned integers (no floating point). */
static inline idx_t int_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Compute a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* undefined; return 0 for safety */

    /* ---------- generate divisors of n ---------- */
    idx_t limit = int_sqrt_u64(n);
    /* maximum number of distinct divisors ≤ 2*sqrt(n) */
    size_t cap = (size_t)(2 * (limit + 1));
    idx_t *divs = (idx_t*)malloc(cap * sizeof(idx_t));
    size_t dcnt = 0;

    for (idx_t i = 1; i <= limit; ++i) {
        if (n % i == 0) {
            divs[dcnt++] = i;
            idx_t j = n / i;
            if (j != i)
                divs[dcnt++] = j;
        }
    }

    /* ---------- sort divisors ascending (simple insertion sort) ---------- */
    for (size_t i = 1; i < dcnt; ++i) {
        idx_t key = divs[i];
        size_t j = i;
        while (j > 0 && divs[j - 1] > key) {
            divs[j] = divs[j - 1];
            --j;
        }
        divs[j] = key;
    }

    /* ---------- compute (n-1)! once as full product ---------- */
    seq_t full = 1;
    for (idx_t k = 2; k <= n - 1; ++k)
        full *= (seq_t)k;

    /* ---------- iterate over divisors, compute (n-1)!/(d-1)! ---------- */
    seq_t sum = 0;
    seq_t fact_up_to = 1;          /* (d-1)! for current divisor */
    idx_t prev_d = 1;              /* previous divisor processed */

    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divs[i];
        /* compute (d-1)! incrementally from prev_d */
        for (idx_t k = prev_d; k < d; ++k)
            fact_up_to *= (seq_t)k;
        prev_d = d;

        /* term = full / fact_up_to */
        seq_t term = full / fact_up_to;
        sum += term;
    }

    free(divs);
    return sum;
}

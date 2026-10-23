/*
 * A133565  a(1)=1. a(n+1) = sum{k=non-isolated divisors of n} a(k). A
 *          non-isolated divisor, k, of n is a positive divisor of n where
 *          (k-1) or (k+1) divides n.
 * https://oeis.org/A133565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9172, u16 n <= 9172, u32 n <= 9172, u64 n <= 9172,
 *            u128 n <= 9172, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = Σ_{k∈non-isolated divisors of (n-1)} a(k)
   a(1) = 1, and a(0) is undefined (returns 0 for safety). */

/* Standard headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static table for memoised values, 1‑based indices. */
static seq_t *memo = NULL;          /* a(n) values */
static bool *computed = NULL;       /* whether memo[i] is set */
static idx_t memo_len = 0;          /* size of allocated arrays */

/* Ensure enough space for indices up to `up_to`. */
static void
ensure_capacity(idx_t up_to)
{
    if (memo_len > up_to)
        return;
    size_t new_cap = (size_t)up_to + 1;           /* indices 0..up_to */
    memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    computed = (bool *)realloc(computed, new_cap * sizeof(bool));
    for (idx_t i = memo_len; i < new_cap; ++i)
        computed[i] = false;
    memo_len = new_cap;
}

/* Compute a(n) for all n up to `upto` that have not yet been computed. */
static void
compute_until(idx_t upto)
{
    idx_t old_len = memo_len;
    ensure_capacity(upto);

    idx_t start = old_len > 1 ? old_len : 1;        /* start at 1 if not yet present */
    for (idx_t n = start; n <= upto; ++n) {
        if (computed[n])
            continue;
        if (n == 1) {                     /* base case */
            memo[1] = (seq_t)1;
            computed[1] = true;
            continue;
        }
        seq_t sum = 0;
        idx_t m = n - 1;                  /* divisor set: divisors of m */
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                /* first divisor d */
                idx_t k = d;
                bool nonisolated = false;
                if (k == 1) {
                    nonisolated = (m % 2 == 0);
                } else {
                    nonisolated = (m % (k - 1) == 0) || (m % (k + 1) == 0);
                }
                if (nonisolated) {
                    /* k < n, so memo[k] already computed */
                    sum += memo[k];
                }

                /* second divisor m/d (if distinct) */
                idx_t k2 = m / d;
                if (k2 != k) {
                    bool nonisolated2 = false;
                    if (k2 == 1) {
                        nonisolated2 = (m % 2 == 0);
                    } else {
                        nonisolated2 = (m % (k2 - 1) == 0) || (m % (k2 + 1) == 0);
                    }
                    if (nonisolated2) {
                        sum += memo[k2];
                    }
                }
            }
        }
        memo[n] = sum;
        computed[n] = true;
    }
}

/* Public sequence function requested by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* not defined, but return 0 safely */
    compute_until(n);
    return memo[n];
}

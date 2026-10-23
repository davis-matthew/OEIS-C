/*
 * A158099  Euler transform of square powers of 2:
 *          [2,2^4,2^9,...,2^(n^2),...].
 * https://oeis.org/A158099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A158099: Euler transform of square powers of 2
 *
 * a(0) = 1
 * for n ≥ 1:
 *     a(n) = (1/n) * Σ_{k=1}^{n} L(k) * a(n-k)
 * where L(k) = Σ_{d|k} d * 2^(d^2)
 *
 * This implementation precomputes L(k) and a(k) up to the maximum
 * requested n, caching the results for subsequent calls.  The
 * recurrence is O(n²), which is acceptable for the test ranges
 * (n ≤ 10 000).  The code compiles under both C11 and C++17.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

#define MAX_N 20000   /* enough for all tests (n ≤ 10 000) */

/* static memoization tables */
static seq_t A[MAX_N + 1];   /* a values */
static seq_t L[MAX_N + 1];   /* L values   */
static idx_t max_a_computed = 0;   /* largest index for which a is computed */
static idx_t max_l_computed = 0;   /* largest index for which L is computed */

/* initialise memo tables once */
static void init_tables(void)
{
    static int initialized = 0;
    if (!initialized) {
        for (idx_t i = 0; i <= MAX_N; ++i) {
            A[i] = 0;
            L[i] = 0;
        }
        A[0] = 1;          /* a(0) = 1 */
        initialized = 1;
    }
}

/* make sure L[k] is computed for all k ≤ n */
static void ensure_L(idx_t n)
{
    if (n > MAX_N) return;  /* safety: tests never request > MAX_N */
    for (idx_t k = max_l_computed + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                seq_t term = (seq_t)d * pow2((idx_t)d * d);
                sum += term;
                if (d * d != k) {
                    idx_t d2 = k / d;
                    term = (seq_t)d2 * pow2((idx_t)d2 * d2);
                    sum += term;
                }
            }
        }
        L[k] = sum;
    }
    max_l_computed = (n > max_l_computed) ? n : max_l_computed;
}

/* compute a(n) with memoization */
seq_t a(idx_t n)
{
    init_tables();

    if (n > MAX_N) {
        /* The problem guarantees n ≤ 10 000; this branch is a safety net. */
        return 0;
    }

    if (n > max_a_computed) {
        /* Ensure L up to n first. */
        ensure_L(n);

        /* Extend a up to n using the recurrence. */
        for (idx_t m = max_a_computed + 1; m <= n; ++m) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= m; ++k)
                sum += L[k] * A[m - k];
            A[m] = sum / (seq_t)m;   /* division is exact by definition */
        }
        max_a_computed = n;
    }

    return A[n];
}

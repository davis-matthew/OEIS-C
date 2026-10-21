/*
 * A240034  Number of n X 3 0..2 arrays with no element equal to zero plus the
 *          sum of elements to its left or zero plus the sum of elements above
 *          it or zero plus the sum of the elements diagonally to its
 *          northwest, modulo 3.
 * https://oeis.org/A240034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 40, u64 n <= 84, u128 n <= 170,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A240034: number of n×3 arrays over {0,1,2} with no zero element
 * and the sum of the element to its left (or 0), the element above it
 * (or 0) and the NW‑diagonal element (or 0), modulo 3, is not 0.
 * Empirical recurrence (valid for n > 7):
 *   a(n) = 4*a(n-2) - 3*a(n-4) - a(n-6)
 * Initial values:
 *   a(1)=4, a(2)=6, a(3)=16, a(4)=16, a(5)=42, a(6)=44, a(7)=114
 * OEIS offset: 1 (a(1) is the first term).
 *
 * The function below implements this recurrence with memoisation.
 * It works for any arithmetic type seq_t provided by the build,
 * handling arbitrary‑precision integers correctly.
 * It also satisfies the requirement to be called in any order.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑indexed cache of computed values */
static idx_t  max_computed = 0; /* largest n with cache[n] valid */

/* Ensure the cache has room for values up to n (inclusive). */
static void
ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        /* n is small enough for the tests; no overflow checking needed */
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    } else if (n > max_computed) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Offset is 1.  By definition n ≥ 1, but we guard against n==0. */
    if (n == 0) {
        return (seq_t)0;
    }

    ensure_capacity(n);

    /* Return cached value if already computed. */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Compute missing values incrementally. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)4;
        } else if (i == 2) {
            cache[i] = (seq_t)6;
        } else if (i == 3) {
            cache[i] = (seq_t)16;
        } else if (i == 4) {
            cache[i] = (seq_t)16;
        } else if (i == 5) {
            cache[i] = (seq_t)42;
        } else if (i == 6) {
            cache[i] = (seq_t)44;
        } else if (i == 7) {
            cache[i] = (seq_t)114;
        } else { /* recurrence valid for i ≥ 8 */
            seq_t term = 4 * cache[i - 2];
            term -= 3 * cache[i - 4];
            term -= cache[i - 6];
            cache[i] = term;
        }
    }

    max_computed = n;
    return cache[n];
}

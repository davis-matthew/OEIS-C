/*
 * A171941  Backwards van Eck transform of A000120.
 * https://oeis.org/A171941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Backwards van Eck transform of A000120 (OEIS A171941)
 *
 *   a(1) = 0
 *   For n > 1:
 *     let v = popcount(n-1)
 *     a(n) = 0                          if no j < n-1 with popcount(j)=v
 *     a(n) = (n-1) - last_occurrence[v] otherwise
 *   where last_occurrence[v] is the largest j < n-1 with popcount(j)=v.
 *
 * The sequence is computed incrementally and the intermediate values are
 * memoized so that calls in arbitrary order are fast.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Compute popcount of an idx_t value.
 * The implementation works in both C and C++. */
static inline int popcnt(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
#endif
}

/* Static state ----------------------------------------------------------- */
static seq_t *seq_vals   = NULL;   /* 1‑based array holding a(1)...a(k) */
static idx_t  seq_cap    = 0;      /* current capacity of seq_vals   */
static idx_t  last_computed = 0;   /* largest n that has been computed */
static idx_t *last_idx   = NULL;   /* last_occurrence for popcounts 0..64 */

/* Ensure seq_vals is large enough for indices up to `need`.
 * The array is grown by allocating a new block, copying the old values
 * with proper copy semantics, and freeing the old block.  This works
 * for both trivial integer types and for classes that require copy
 * constructors / destructors.
 */
static void ensure_storage(idx_t need)
{
    /* Grow seq_vals if necessary */
    if (seq_cap < need + 1) {
        idx_t new_cap = need + 1;
        seq_t *tmp = NULL;

#ifdef __cplusplus
        tmp = new seq_t[new_cap];          /* use new/delete for C++ */
#else
        tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif

        /* Copy the values that we already have */
        for (idx_t k = 1; k <= last_computed; ++k)
            tmp[k] = seq_vals[k];

        /* Release the old array */
        if (seq_vals) {
#ifdef __cplusplus
            delete[] seq_vals;
#else
            free(seq_vals);
#endif
        }

        seq_vals = tmp;
        seq_cap  = new_cap;
    }

    /* Initialise the last_idx table exactly once */
    if (!last_idx) {
        /* 65 buckets are enough for popcount of a 64‑bit integer */
        last_idx = (idx_t*)calloc(65, sizeof(idx_t));
    }
}

/* ----------------------------------------------------------------------- *
 *  Main sequence function                                               *
 * ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1 – guard against 0            */
        return (seq_t)0;

    ensure_storage(n);

    /* If we already have a(n), simply return it */
    if (n <= last_computed)
        return seq_vals[n];

    /* Compute values from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        idx_t target = i - 1;               /* n-1 in the definition            */
        int v = popcnt(target);             /* popcount(n-1)                      */
        idx_t prev = last_idx[v];           /* previous index with same popcount  */
        seq_vals[i] = (prev == 0) ? (seq_t)0
                                  : (seq_t)(target - prev);
        last_idx[v] = target;               /* update last occurrence             */
    }

    last_computed = n;
    return seq_vals[n];
}

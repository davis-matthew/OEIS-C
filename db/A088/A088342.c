/*
 * A088342  Let T = Sum_{k >= 1} k^(k-1)*x^k be the g.f. for rooted labeled
 *          trees (A000169); sequence has g.f. T/(1-T).
 * https://oeis.org/A088342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} k^(k-1) * a(n-k) with a(0)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, ensure_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088342 – forests of rooted trees with interval components. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/ensure_arr.h"

/* --------------------------------------------------------------------- */
/* Memoisation tables: the largest n for which the values are known.      */
/* --------------------------------------------------------------------- */
static seq_t *a_cache    = NULL;   /* a_cache[0..]  */
static size_t a_cap       = 0;      /* current allocation size */
static seq_t *T_cache    = NULL;   /* T_cache[k] = k^(k-1), k>=1   */
static size_t T_cap       = 0;
static idx_t  max_computed = 0;     /* highest n actually filled */

/* --------------------------------------------------------------------- */
/* Ensure that an array can store indices 0..n inclusive.                */

/* --------------------------------------------------------------------- */
/* Return the n-th term of A088342.  n>=1.  a(0) is defined as 1 for      */
/* the recurrence.                                                        */
/* --------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)                     /* for internal use – not a public value */
        return (seq_t)1;

    /* Allocate or grow the tables if needed */
    ensure_arr(n, &a_cache, &a_cap);
    ensure_arr(n, &T_cache, &T_cap);

    /* Initialise a_cache[0] exactly once */
    if (max_computed == 0) {
        a_cache[0] = (seq_t)1;      /* required by the recurrence */
    }

    /* Compute values up to n (can be called in any order) */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            if (T_cache[k] == (seq_t)0)                /* not yet known */
                T_cache[k] = big_pow((seq_t)k, (seq_t)(k - 1));
            sum += T_cache[k] * a_cache[i - k];
        }
        a_cache[i] = sum;
    }
    max_computed = n;
    return a_cache[n];
}

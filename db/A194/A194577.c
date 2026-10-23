/*
 * A194577  a(1)=1 and for n>=2 Sum_{k=1..n} a(k)*(-1)^floor(n/k)=0.
 * https://oeis.org/A194577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 47, i16 n <= 1535, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A194577
   a(1) = 1
   For n >= 2:   sum_{k=1..n} a(k)*(-1)^floor(n/k) = 0
   Equivalently:  a(n) = sum_{k=1}^{n-1} a(k)*(-1)^floor(n/k)
   (the term for k = n has -a(n) and is moved to the RHS)
   This implementation uses memoisation: once a value has been
   computed it is stored and reused for later calls.
*/

#include <stdlib.h>
#include <stdint.h>

/* idx_t and seq_t are predefined by the build system (e.g. uint64_t, int8_t, __int128, ...). */

/* Static storage for memoisation */
static seq_t *a_vals = NULL;   /* a_vals[1 .. max_n]  (a_vals[0] unused) */
static idx_t   max_n = (idx_t)0;   /* largest n for which a_vals[n] is valid */

/* Reallocate the memoisation tables up to at least n (inclusive). */
static void
ensure_capacity(idx_t n)
{
    if (n <= max_n)
        return;   /* already ready */

    /* Allocate or grow the storage.  We keep index 0 unused. */
    size_t new_cap = (size_t)n + 1;      /* size_t may hold n+1 */
    seq_t *new_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!new_vals) {
        /* Allocation failed – abort with a diagnostic. */
        abort();
    }
    a_vals = new_vals;

    /* Init first element (unused) and a(1). */
    if (max_n == (idx_t)0) {
        a_vals[0] = (seq_t)0;
        a_vals[1] = (seq_t)1;
        max_n = 1;
    }

    /* Compute missing terms */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < m; ++k) {
            /* (-1)^floor(m/k) == -1 if floor(m/k) is odd, +1 otherwise */
            seq_t sign = ((m / k) & (idx_t)1) ? -1 : 1;
            sum += a_vals[k] * sign;
        }
        a_vals[m] = sum;
    }
    max_n = n;
}

seq_t
a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0; /* undefined for n <= 0, but return 0 */

    ensure_capacity(n);
    return a_vals[n];
}

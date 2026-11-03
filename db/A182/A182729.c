/*
 * A182729  Square array T(n,k) = (n*k-1)*A000041(n) read by antidiagonals
 *          upwards.
 * https://oeis.org/A182729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A182729: Square array T(n,k) = (n*k-1)*P(n) read by antidiagonals upward.
   a(n) is obtained by mapping index n to (row = n_val, col = k_val)
   and returning (n_val*k_val-1)*P(n_val), where P is the partition
   function given by A000041.  The partition numbers are computed on‑demand
   using Euler's recurrence with generalized pentagonal numbers. */

#include <stdlib.h>
#include <stdint.h>

#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/mem.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* Ensure `p` contains partition numbers P(0)…P(max_n).  */
static seq_t *p      = NULL;  /* P(n) stored at p[n]  */
static size_t p_size = 0;     /* current allocated size     */

/* Helper to extend the partition table up to `max_n`.  */
static void ensure_partition(size_t max_n)
{
    if (max_n < p_size)
        return;

    size_t old_size = p_size;
    size_t new_size = max_n + 1;

    /* Reallocate using seq_realloc so that arbitrary seq_t types are handled. */
    seq_t *tmp = seq_realloc(p, old_size, new_size);
    if (!tmp) abort();    /* memory allocation failed */
    p = tmp;
    p_size = new_size;

    /* Initialize the first element if we are starting fresh. */
    if (old_size == 0) {
        p[0] = (seq_t)1;              /* P(0) = 1 by convention */
        old_size = 1;
    }

    /* Compute missing partitions using Euler's pentagonal recurrence. */
    for (size_t n = old_size; n <= max_n; ++n) {
        seq_t sum = 0;
        for (size_t k = 1; ; ++k) {
            size_t g1 = k * (3 * k - 1) / 2;
            if (n < g1) break;

            if ((k & 1) == 1) sum += p[n - g1];
            else              sum -= p[n - g1];

            size_t g2 = k * (3 * k + 1) / 2;
            if (n < g2) continue;

            if ((k & 1) == 1) sum += p[n - g2];
            else              sum -= p[n - g2];
        }
        p[n] = sum;
    }
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined, but keep safe */
        return (seq_t)0;

    /* Find the diagonal d such that d*(d+1)/2 >= n.  */
    idx_t d = 0;
    while (d * (d + 1) / 2 < n)
        ++d;

    /* Number of elements before this diagonal.  */
    idx_t prev = d * (d - 1) / 2;

    /* Position inside the diagonal (1 … d).  */
    idx_t offset = n - prev;

    /* Compute row (n_val) and column (k_val) inside T(n,k).  */
    idx_t n_val = d - offset + 1;   /* rows decrease inside a diagonal */
    idx_t k_val = offset;           /* columns increase inside a diagonal */

    /* Ensure the partition table covers n_val.  */
    ensure_partition((size_t)n_val);

    /* Compute the term: (n_val*k_val - 1) * P(n_val).  */
    seq_t part = p[n_val];
    seq_t term = (seq_t)n_val * (seq_t)k_val - (seq_t)1;
    term *= part;
    return term;
}

/*
 * A038490  Sums of 2 distinct powers of 11.
 * https://oeis.org/A038490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth smallest value of 11^i + 11^j with i < j, i, j ≥
 *            0. The function computes this by generating all such sums up to
 *            the necessary exponent, sorting them with qsort, and returning
 *            the n‑th smallest element. O(n log n) time, O(n) memory.
 *            Compatible with both C11 and C++17 builds
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 45, u64 n <= 171, u128 n <= 699,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 9956 (2026-11-03)
 * Helpers:   ipow_u64_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#  include <new>
#endif
#include "../utils/mem.h"

/* Utility to compute 11^exp.  The function is provided by the build
   environment and returns a value of type seq_t. */
#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* Comparison routine for seq_t values used by qsort. */

/* The public sequence function.  External linkage is required. */
seq_t a(idx_t n)
{
    /* Off‑by‑one handling: a(1) is the first term. */
    if (n == 0U)
        return (seq_t)0;

    /* Find the minimal k such that k*(k+1)/2 ≥ n. */
    idx_t k = 0;
    while ((unsigned __int128)k * (k + 1) / 2 < n)
        ++k;

    /* Total number of distinct pairs (i,j) with 0 ≤ i < j ≤ k. */
    size_t total_pairs = (size_t)((unsigned __int128)k * (k + 1) / 2);

    /* Allocate space for the sums.  We use malloc to avoid
       invoking constructors for potentially non‑trivial seq_t. */
    seq_t *arr = mem_new(seq_t, total_pairs);
    if (!arr)
        return (seq_t)0;          /* allocation failure – give up */

    /* Generate all sums 11^i + 11^j with i < j. */
    size_t idx = 0;
    for (idx_t i = 0; i < k; ++i) {
        seq_t pow_i = ipow_u64_idx(11, i);
        for (idx_t j = i + 1; j <= k; ++j) {
            seq_t pow_j = ipow_u64_idx(11, j);
            seq_t sum = pow_i + pow_j;
            /* Construct the seq_t in the allocated memory. */
#ifdef __cplusplus
            *(arr + idx) = (seq_t)(sum);
#else
            arr[idx] = sum;
#endif
            ++idx;
        }
    }

    /* Sort the sums to obtain them in ascending order. */
    mem_qsort(arr, total_pairs, sizeof(seq_t), cmp_seq_t);

    /* Extract the requested term. */
    seq_t result = arr[n - 1U];

#ifdef __cplusplus
    /* Call destructors for the seq_t objects before freeing the
       raw memory.  In a plain C build this block is skipped. */
    for (size_t i = 0; i < total_pairs; ++i)
(void)0;  /* mem_free destroys the elements */
#endif

    mem_free(arr);
    return result;
}

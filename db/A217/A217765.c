/*
 * A217765  Square array T, read by antidiagonals: T(n,k) = 0 if n-k >=3 or if
 *          k-n >= 6, T(2,0) = T(1,0) = T(0,0) = T(0,1) = T(0,2) = T(0,3) =
 *          T(0,4) = T(0,5) = 1, T(n,k) = T(n-1,k) + T(n,k-1).
 * https://oeis.org/A217765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A217765 – Square array T read by antidiagonals
 *
 * The sequence is defined by the square array T(n,k) with
 *   T(n,k) = 0 if n-k >= 3   or   k-n >= 6
 *   T(0,k) = 1 for 0 <= k <= 5, 0 otherwise
 *   T(n,0) = 1 for n <= 2,      0 otherwise
 *   T(n,k) = T(n-1,k) + T(n,k-1)   when the band condition holds
 *
 * a(n) is the n-th element in the antidiagonal traversal:
 *   let d satisfy d(d+1)/2 <= n < (d+1)(d+2)/2,
 *   let offset = n - d(d+1)/2,
 *   a(n) = T(offset , d-offset).
 *
 * The implementation memoises T(n,k) in a dynamically resized
 * 2‑D array.  It handles all unsigned arithmetic safely and
 * works for arbitrary‑precision seq_t or any unsigned integer
 * size up to 128 bits.  No globals from the build are used
 * except seq_t and idx_t which are provided by the judge.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ----- internal state for the T(n,k) table ----- */
static seq_t *T_data   = NULL;   /* flattened 2‑D array of values   */
static bool  *T_seen   = NULL;   /* flattened 2‑D array of computed flags */
static idx_t  T_max_i  = 0;       /* maximum i currently stored (inclusive) */
static idx_t  T_max_j  = 0;       /* maximum j currently stored (inclusive) */

/* Expand the table so that indices i,j are within bounds.
 * The new size grows by doubling until it covers the requested
 * indices.  All new entries are cleared to zero/false. */
static void
ensure_T_capacity(idx_t i, idx_t j)
{
    /* If already enough space, nothing to do. */
    if (i <= T_max_i && j <= T_max_j)
        return;

    /* Compute new dimensions by repeatedly doubling + 1. */
    idx_t new_i = T_max_i;
    idx_t new_j = T_max_j;
    while (new_i < i) new_i = new_i * 2 + 1;
    while (new_j < j) new_j = new_j * 2 + 1;

    size_t new_size = (size_t)(new_i + 1) * (new_j + 1);

    /* Allocate new storage and zero it. */
    seq_t *new_data = (seq_t *)calloc(new_size, sizeof(seq_t));
    bool  *new_seen = (bool  *)calloc(new_size, sizeof(bool));
    if (!new_data || !new_seen)
        exit(EXIT_FAILURE);   /* allocation failure */

    /* Copy old data to the new layout. */
    if (T_data != NULL) {
        for (idx_t r = 0; r <= T_max_i; ++r) {
            size_t src_off = (size_t)r * (T_max_j + 1);
            size_t dst_off = (size_t)r * (new_j + 1);
            for (idx_t c = 0; c <= T_max_j; ++c) {
                new_data[dst_off + c] = T_data[src_off + c];
                new_seen[dst_off + c] = T_seen[src_off + c];
            }
        }
        free(T_data);
        free(T_seen);
    }

    T_data = new_data;
    T_seen = new_seen;
    T_max_i = new_i;
    T_max_j = new_j;
}

/* Recursive, memoised evaluation of T(n,k).  The function
 * uses the band condition to avoid unnecessary recursion
 * and handles the special borders directly. */
static seq_t
T_at(idx_t i, idx_t j)
{
    /* Handle the simple border cases first. */
    if (i == 0)
        return (j <= 5) ? (seq_t)1 : (seq_t)0;

    if (j == 0)
        return (i <= 2) ? (seq_t)1 : (seq_t)0;

    /* Band condition: zero outside the allowed strip. */
    if (i >= j) {
        if (i - j >= 3)
            return (seq_t)0;
    } else {
        if (j - i >= 6)
            return (seq_t)0;
    }

    /* Compute (or retrieve) the value inside the band. */
    ensure_T_capacity(i, j);
    size_t pos = (size_t)i * (T_max_j + 1) + j;
    if (T_seen[pos])
        return T_data[pos];

    /* Recursive definition. */
    seq_t val = T_at(i - 1, j) + T_at(i, j - 1);
    T_data[pos] = val;
    T_seen[pos] = true;
    return val;
}

/* ------------------------------------------------------------------
 * Main sequence function
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Determine the antidiagonal number d such that
     * d(d+1)/2 <= n < (d+1)(d+2)/2. */
    idx_t d = 0;
    while (((unsigned __int128)(d + 1) * (d + 2)) / 2 <= n)
        ++d;

    idx_t base   = (unsigned __int128)d * (d + 1) / 2;
    idx_t offset = n - base;          /* 0 <= offset <= d */
    idx_t i = offset;                 /* row on this diagonal   */
    idx_t j = d - offset;             /* column on this diagonal*/

    return T_at(i, j);
}

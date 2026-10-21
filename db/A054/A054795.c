/*
 * A054795  Numbers of form 23+n^2+n or 23+2*n^2.
 * https://oeis.org/A054795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A054795 – numbers of form 23 + n^2 + n or 23 + 2 n^2 (offset 1) */
/* The sequence is obtained by merging the two increasing sequences:
 *   a_idx(n) = 23 + n(n+1)          (triangular offset 23)
 *   b_idx(n) = 23 + 2 n²            (double‑square offset 23)
 * and deleting duplicates.  The implementation keeps a sorted array of
 * values as needed and expands it on demand.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq_array = NULL;     /* sorted union of the two sequences */
static size_t seq_len  = 0;         /* number of elements stored so far */
static size_t seq_cap  = 0;         /* allocated capacity of seq_array */

static idx_t a_idx = 0;             /* next index to use for 23 + n(n+1)  */
static idx_t b_idx = 0;             /* next index to use for 23 + 2 n²  */

static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();                     /* memory allocation failed */
    seq_array = new_ptr;
    seq_cap   = new_cap;
}

/* Build the union up to at least 'n' elements.  All new values are appended. */
static void grow_to(idx_t n)
{
    while (seq_len < n) {
        /* Compute current candidates from both sequences */
        seq_t valA = (seq_t)23 + (seq_t)a_idx * ((seq_t)a_idx + 1);
        seq_t valB = (seq_t)23 + (seq_t)2 * (seq_t)b_idx * (seq_t)b_idx;

        /* Select the smaller value (or either if equal) */
        seq_t next;
        bool equal = false;
        if (valA < valB) {
            next = valA;
        } else if (valB < valA) {
            next = valB;
        } else {
            next = valA;      /* valA == valB */
            equal = true;
        }

        /* Append the chosen value */
        ensure_capacity(seq_len + 1);
        seq_array[seq_len++] = next;

        /* Advance indices that contributed the chosen value */
        if (valA == next)
            ++a_idx;
        if (valB == next)
            ++b_idx;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for 0, but return 0 for safety */
        return (seq_t)0;

    grow_to(n);
    return seq_array[n - 1];   /* a(1) is the first element */
}

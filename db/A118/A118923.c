/*
 * A118923  Triangle T(n,k) built by placing T(n,0)=A000012(n) in the left
 *          edge, T(n,n)=A079978(n) on the right edge and filling the body
 *          with the Pascal recurrence T(n,k) = T(n-1,k) + T(n-1,k-1).
 * https://oeis.org/A118923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the n-th element (0‑based) obtained by reading triangle
 *            T row by row, where T(0,0) = 1, T(n,0) = 1, T(n,n) = 1 if n ≡ 0
 *            (mod 3) else 0, T(n,k) = T(n‑1,k) + T(n‑1,k‑1) for 0 < k < n
 * Range:     u8 n <= 70, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static mutable state for memoisation */
static seq_t  *seq_cache = NULL;     /* flattened sequence */
static idx_t   seq_len   = 0;        /* number of elements stored */
static idx_t   seq_cap   = 0;        /* allocated capacity */

static seq_t  *prev_row  = NULL;     /* previous row values */
static idx_t   prev_len  = 0;        /* length of previous row */

/* initialise the first row (row 0) */
static void init_once(void)
{
    if (seq_len != 0) return;            /* already initialised */

    /* allocate capacity for the first value */
    seq_cap = 1;
    seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));

    /* row 0 */
    prev_len = 1;
    prev_row = (seq_t*)malloc(prev_len * sizeof(seq_t));
    prev_row[0] = (seq_t)1;

    /* append the single element of row 0 to the cache */
    seq_cache[seq_len++] = prev_row[0];
}

/* grow the flattened cache if needed */
static void ensure_cache(idx_t n)
{
    while (seq_len <= n) {
        /* current row index equals prev_len (because row 0 has length 1) */
        idx_t row = prev_len;      /* row number: length of previous row */
        idx_t new_len = row + 1;   /* length of the new row */

        /* allocate memory for the new row */
        seq_t *new_row = (seq_t*)malloc(new_len * sizeof(seq_t));

        /* left edge */
        new_row[0] = (seq_t)1;

        /* interior entries */
        for (idx_t k = 1; k < row; ++k) {
            new_row[k] = prev_row[k-1] + prev_row[k];
        }

        /* right edge */
        new_row[row] = (row % 3 == 0) ? (seq_t)1 : (seq_t)0;

        /* ensure sufficient capacity for the cache */
        if (seq_len + new_len > seq_cap) {
            seq_cap = (seq_cap == 0) ? 1 : seq_cap * 2;
            while (seq_len + new_len > seq_cap) seq_cap *= 2;
            seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
        }

        /* append the new row to the flattened cache */
        for (idx_t k = 0; k < new_len; ++k) {
            seq_cache[seq_len++] = new_row[k];
        }

        /* advance to the next row */
        free(prev_row);
        prev_row = new_row;
        prev_len = new_len;
    }
}

seq_t a(idx_t n)
{
    init_once();          /* make sure the first row is set up */
    ensure_cache(n);      /* extend the cache up to at least index n */
    return seq_cache[n];
}

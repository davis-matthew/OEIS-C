/*
 * A223516  Triangle T(n,k) represents the coefficients of (x^14*d/dx)^n,
 *          where n=1,2,3,...; generalization of Stirling numbers of second
 *          kind A008277, Lah-numbers A008297.
 * https://oeis.org/A223516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 28, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A223516
 *
 *  Triangle T(n,k) of the coefficients of (x^14*d/dx)^n
 *      T(n,k) = (k + 13*(n-1)) * T(n-1,k) + T(n-1,k-1)
 *  with T(1,1) = 1 and T(n,k)=0 when k<1 or k>n.
 *
 *  The sequence a(n) is obtained by flattening the rows of the triangle
 *  (row‑1 written first, then row‑2, etc.).  For example:
 *          1 | 14,1 | 378,42,1 | 15120,2100,84,1 | …
 *
 *  Function a() returns a(n) for any n ≥ 1.  The function memoises all
 *  previously computed terms so that subsequent calls are O(1).
 */

#include <stdlib.h>

/* --- helper for dynamic arrays ------------------------------------- */
static void ensure_capacity(void **ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (needed > *cap) {
        size_t new_cap = (*cap > 0) ? *cap * 2 : 1;
        while (new_cap < needed)
            new_cap *= 2;
        void *tmp = realloc(*ptr, new_cap * elem_size);
        if (!tmp)
            return;   /* out of memory – the caller will fail gracefully */
        *ptr   = tmp;
        *cap   = new_cap;
    }
}

/* --- memoised flat triangle ---------------------------------------- */
static seq_t *flat      = NULL;   /* all terms in order */
static size_t flat_cap  = 0;
static size_t flat_len  = 0;      /* number of terms stored */

static seq_t *prev_row  = NULL;   /* previous row values (1‑based) */
static size_t prev_cap  = 0;

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* trivial case -------------------------------------------------- */
    if (n < 1)
        return 0;   /* undefined for n<1 – caller should avoid this */

    /* already computed? --------------------------------------------- */
    if ((size_t)n <= flat_len)
        return flat[(size_t)n - 1];

    /* initialise on first call -------------------------------------- */
    if (!flat) {
        /* allocate and store first term a(1) = 1 */
        ensure_capacity((void**)&flat, &flat_cap, 1, sizeof(seq_t));
        flat[0] = 1;
        flat_len = 1;

        /* initialise first row in prev_row */
        ensure_capacity((void**)&prev_row, &prev_cap, 2, sizeof(seq_t));
        prev_row[1] = 1;   /* T(1,1) */
    }

    /* generate rows until we reach the requested index n ----------- */
    idx_t current_row = (idx_t)flat_len;   /* number of terms already stored */
    /* Since rows are 1‑based, the number of terms after row r is
     * 1 + 2 + … + r = r*(r+1)/2.  Compute the current row number r such
     * that the next needed row will be r+1.  Using the triangular numbers
     * we can find the smallest r with r*(r+1)/2 >= flat_len.  However
     * for simplicity we just rebuild rows incrementally. */
    idx_t r = 1;                   /* last row computed   */
    /* find last row from flat_len */
    for (r = 1; ; ++r) {
        if (r * (r + 1) / 2 >= flat_len)
            break;
    }
    /* r is now the last row already computed */
    for (++r; r <= (idx_t)n; ++r) {
        /* ensure we have space for the new row in prev_row */
        ensure_capacity((void**)&prev_row, &prev_cap, r + 1, sizeof(seq_t));

        /* new row is r; compute each element in descending order
         * so that we can use the in‑place recurrence. */
        prev_row[r] = 0;   /* safety: old value might be garbage */
        for (idx_t k = r; k >= 1; --k) {
            seq_t factor = (seq_t)k + (seq_t)13 * (seq_t)(r - 1);
            seq_t term1 = factor * prev_row[k];   /* (k+13*(r-1))*T(r-1,k) */
            seq_t term2 = (k > 1) ? prev_row[k - 1] : (seq_t)0; /* T(r-1,k-1) */
            prev_row[k] = term1 + term2;
        }

        /* append the new row to the flat array */
        ensure_capacity((void**)&flat, &flat_cap, flat_len + r, sizeof(seq_t));
        for (idx_t k = 1; k <= r; ++k)
            flat[flat_len++] = prev_row[k];
    }

    return flat[(size_t)n - 1];
}

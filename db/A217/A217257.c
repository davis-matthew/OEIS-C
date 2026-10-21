/*
 * A217257  Square array T, read by antidiagonals: T(n,k) = 0 if n-k >= 1 or
 *          if k-n >= 7, T(0,0) = T(0,1) = T(0,2) = T(0,3) = T(0,3) = T(0,4) =
 *          T(0,5) = T(0,6) = 1, T(n,k) = T(n-1,k) + T(n,k-1).
 * https://oeis.org/A217257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A217257 – Square array T read by antidiagonals.
 *
 *  T(n,k) = 0                           if n-k >= 1  (n > k)
 *  T(n,k) = 0                           if k-n >= 7  (k >= n+7)
 *  T(0,k) = 1   for 0 <= k <= 6
 *  T(n,k) = T(n-1,k) + T(n,k-1)   for 0 <= n <= k <= n+6
 *
 *  The sequence a(n) is obtained by listing the entries of T
 *  in antidiagonal order:  a(0)=T(0,0), a(1)=T(0,1), a(2)=T(1,0), a(3)=T(0,2), … .
 *
 *  This implementation stores each band of 7 columns for every row.
 *  Rows are computed on demand with the recurrence
 *
 *      T[n][d] = (d+1 <= 6 ? T[n-1][d+1] : 0)   +   (d > 0 ? T[n][d-1] : 0)
 *
 *  where d = k - n (0 … 6).  The routine is memoised and works for
 *  arbitrary-precision seq_t as well as for built‑in integer types.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t **T = NULL;      /* T[i][d] – row i, difference d = k-i (0…6) */
static idx_t max_i = 0;       /* largest row that has been initialised */

/* Ensure that rows 0..required_i of T are computed and stored.  */
static void ensure_rows(idx_t required_i)
{
    /* Allocate the first row if necessary */
    if (T == NULL) {
        T = (seq_t **)malloc(sizeof(seq_t *));
        if (!T) abort();
        T[0] = (seq_t *)malloc(7 * sizeof(seq_t));
        if (!T[0]) abort();
        for (idx_t d = 0; d < 7; ++d) T[0][d] = (seq_t)1;
        max_i = 0;
    }

    if (required_i <= max_i) return;   /* already initialised */

    /* Grow the array of row pointers */
    T = (seq_t **)realloc(T, (required_i + 1) * sizeof(seq_t *));
    if (!T) abort();

    /* Compute new rows */
    for (idx_t r = max_i + 1; r <= required_i; ++r) {
        T[r] = (seq_t *)malloc(7 * sizeof(seq_t));
        if (!T[r]) abort();

        if (r == 0) {
            /* already handled above, but keep for clarity */
            for (idx_t d = 0; d < 7; ++d) T[0][d] = (seq_t)1;
        } else {
            seq_t *prev = T[r - 1];

            /* d = 0 : depends only on the previous row's d+1 = 1 */
            T[r][0] = prev[1];

            for (idx_t d = 1; d < 7; ++d) {
                seq_t left = (d + 1 < 7) ? prev[d + 1] : (seq_t)0;
                seq_t up   = T[r][d - 1];
                T[r][d] = left + up;
            }
        }
    }
    max_i = required_i;
}

/* Return the n-th term of sequence A217257. */
seq_t a(idx_t n)
{
    /* Find the antidiagonal 's' such that
     * sum_{t=0}^{s-1} (t+1) <= n < sum_{t=0}^{s} (t+1)
     */
    idx_t s = 0;
    idx_t cum = 0;
    while (cum + (s + 1) <= n) {
        cum += s + 1;
        ++s;
    }

    idx_t offset = n - cum;          /* position within diagonal s (0 … s) */
    idx_t i = offset;                /* row index                 */
    idx_t j = s - offset;            /* column index              */

    /* Outside the band 0 <= j-i <= 6 → value is 0 */
    if (j < i || j > i + 6) return (seq_t)0;

    idx_t delta = j - i;             /* 0 … 6 */
    ensure_rows(i);
    return T[i][(size_t)delta];
}

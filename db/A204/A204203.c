/*
 * A204203  Triangle based on (0,1/4,1) averaging array.
 * https://oeis.org/A204203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Triangle T(n,k) averaging array; a(n) is the flattened
   row‑wise triangle.  Recurrence:
          T(n,k) = T(n‑1,k) + 3*T(n‑1,k‑1) – 2*T(n‑2,k‑1) – 2*T(n‑2,k‑2)
   with T(1,1)=1, T(2,1)=1, T(2,2)=5 and T(n,k)=0 if k<1 or k>n. */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1 */

    /* Find the row r that contains the n‑th element (1‑based rows). */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        __int128 v = (__int128)mid * (mid + 1) / 2;
        if (v >= n) hi = mid;
        else        lo = mid + 1;
    }
    idx_t r = lo;

    /* Column index within that row (1‑based). */
    idx_t prev_total = (idx_t)(((__int128)(r - 1) * r) / 2);
    idx_t k = n - prev_total;   /* 1 <= k <= r */

    /* Static memoisation state. */
    static seq_t *data      = NULL;   /* flat array of T values  */
    static idx_t max_row    = 0;      /* highest computed row     */
    static size_t capacity  = 0;      /* current allocation size  */

    /* Resize array if needed to hold up to row r. */
    size_t need_cap = (size_t)(((__int128)r * (r + 1)) / 2);
    if (need_cap > capacity) {
        seq_t *tmp = (seq_t *)realloc(data, need_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        data = tmp;
        capacity = need_cap;
    }

    /* Initialise the first two rows if not yet done. */
    if (max_row < 1) {
        data[0] = (seq_t)1;           /* T(1,1) */
        max_row = 1;
    }
    if (max_row < 2) {
        /* Ensure space for rows 1 and 2. */
        if (capacity < 3) {
            seq_t *tmp = (seq_t *)realloc(data, 3 * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            data = tmp;
            capacity = 3;
        }
        data[1] = (seq_t)1;   /* T(2,1) */
        data[2] = (seq_t)5;   /* T(2,2) */
        max_row = 2;
    }

    /* Compute rows from max_row+1 up to the required row r. */
    for (idx_t row = max_row + 1; row <= r; ++row) {
        size_t start_row   = (size_t)(((__int128)(row - 1) * row) / 2);
        size_t start_prev1 = (size_t)(((__int128)(row - 2) * (row - 1)) / 2);
        size_t start_prev2 = (row >= 3)
            ? (size_t)(((__int128)(row - 3) * (row - 2)) / 2)
            : 0;

        for (idx_t col = 1; col <= row; ++col) {
            seq_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

            if (col <= row - 1)                       /* T(n-1,k)   */
                t1 = data[start_prev1 + (col - 1)];
            if (col >= 2)                             /* T(n-1,k-1) */
                t2 = data[start_prev1 + (col - 2)];
            if (col >= 2 && col - 1 <= row - 2)       /* T(n-2,k-1) */
                t3 = data[start_prev2 + (col - 2)];
            if (col >= 3)                             /* T(n-2,k-2) */
                t4 = data[start_prev2 + (col - 3)];

            data[start_row + (col - 1)] =
                t1 + (seq_t)3 * t2 - (seq_t)2 * t3 - (seq_t)2 * t4;
        }
    }

    max_row = r;   /* we have now computed up to row r */

    /* Return the requested element. */
    size_t start_row = (size_t)(((__int128)(r - 1) * r) / 2);
    return data[start_row + (k - 1)];
}

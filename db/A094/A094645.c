/*
 * A094645  Triangle of generalized Stirling numbers of the first kind read by
 *          rows: T(n, k) = (-1)^(n+k)*Sum_{m=0..n} Stirling1(n, m) *
 *          binomial(m, k), where Stirling1 is A048994.
 * https://oeis.org/A094645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the flattening of the triangle T(n,k) read by rows. The
 *            triangle T is defined by the recurrence T(0,0) = 1, T(n,k) =
 *            T(n-1,k-1) + (n-2)*T(n-1,k) for n >= 1, k >= 0 with T(n,-1)=0
 *            and T(n,k)=0 for n < k. For an index n >= 0, let r be the unique
 *            integer with r*(r+1)/2 <= n < (r+1)*(r+2)/2 a...
 * Range:     i8 n <= 29, i16 n <= 55, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r and column c corresponding to the flattened index n */
    idx_t r = 0;
    while ( ((r + 1) * (r + 2)) / 2 <= n )
        ++r;
    /* Now r is the row such that the first element of the row is at
       index r*(r+1)/2. */
    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;                     /* 0 <= c <= r */

    /* Special case: the very first element of the triangle. */
    if (r == 0)
        return (seq_t)1;

    /* Reusable buffers for the current and next row of length up to c+1.
       They are allocated once and reused across calls to avoid repeated
       allocations. */
    static seq_t *buf1 = NULL;
    static seq_t *buf2 = NULL;
    static size_t buf_cap = 0;                   /* current capacity of the buffers */

    size_t need = (size_t)c + 1;                  /* columns 0..c inclusive */
    if (need > buf_cap) {
        free(buf1);
        free(buf2);
        buf1 = (seq_t *)malloc(need * sizeof(seq_t));
        buf2 = (seq_t *)malloc(need * sizeof(seq_t));
        buf_cap = need;
    }

    seq_t *cur = buf1;     /* current row vector */
    seq_t *next = buf2;    /* buffer for the next row */

    /* Initialize row 0 */
    cur[0] = (seq_t)1;
    size_t cur_len = 1;    /* only index 0 is defined */

    /* Iterate from row 1 up to row r */
    for (idx_t i = 1; i <= r; ++i) {
        /* The next row has length min(i, c) + 1 */
        size_t next_len = (size_t)((i < c) ? i : c) + 1;

        for (size_t k = 0; k < next_len; ++k) {
            if (k == 0) {
                /* k == 0: T(i,0) = (i-2) * T(i-1,0) */
                seq_t mult = (seq_t)((int64_t)i - 2);
                next[0] = mult * cur[0];
            } else if (k == next_len - 1 && k == (size_t)i) {
                /* k == i: T(i,i) = T(i-1,i-1) */
                next[k] = cur[k - 1];
            } else {
                /* General recurrence:
                   T(i,k) = T(i-1,k-1) + (i-2)*T(i-1,k) */
                seq_t val_prev_k   = (k < cur_len) ? cur[k] : 0;   /* T(i-1,k) */
                seq_t val_prev_km1 = cur[k - 1];                   /* T(i-1,k-1) */
                seq_t mult = (seq_t)((int64_t)i - 2);
                next[k] = val_prev_km1 + mult * val_prev_k;
            }
        }

        /* Prepare for the next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
        cur_len = next_len;
    }

    /* After the loop, cur points to row r */
    return cur[c];
}

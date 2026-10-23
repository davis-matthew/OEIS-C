/*
 * A126970  Triangle T(n,k), 0 <= k <= n, read by rows given by: T(0,0)=1,
 *          T(n,k)=0 if k < 0 or if k > n, T(n,0) = T(n-1,1), T(n,k) =
 *          T(n-1,k-1) + 3*T(n-1,k) + T(n-1,k+1) for k >= 1.
 * https://oeis.org/A126970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the nth entry of the triangle T(n,k) read row‑by‑row,
 *            defined by T(0,0) = 1, T(n,k) = 0 if k<0 or k>n, T(n,0) =
 *            T(n-1,1), T(n,k) = T(n-1,k-1) + 3*T(n-1,k) + T(n-1,k+1) for
 *            k>=1. The sequence a(n) is obtained by scanning the triangle
 *            left‑to‑right row by row. Initial values: a(0)=1, a(1)=0,...
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 153, u64 n <= 496,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r such that row r starts at index offset
       and contains n.  Also compute the column k = n - offset.      */
    idx_t r = 0;
    idx_t offset = 0;
    while (offset + (r + 1) <= n) {
        offset += r + 1;
        ++r;
    }
    idx_t k = n - offset;

    /* Compute the triangle rows up to row r.  For each row we
       keep only the previous row in memory.                               */
    size_t prev_len = 1;
    seq_t *prev = (seq_t *)calloc(prev_len, sizeof(seq_t));
    if (!prev) return 0;  /* allocation failure, undefined behavior */
    prev[0] = 1;          /* T(0,0) */

    for (idx_t i = 1; i <= r; ++i) {
        size_t cur_len = (size_t)i + 1;
        seq_t *cur = (seq_t *)calloc(cur_len, sizeof(seq_t));
        if (!cur) { free(prev); return 0; }

        /* T(i,0) = T(i-1,1)  (0 if out of bounds) */
        cur[0] = (prev_len > 1) ? prev[1] : (seq_t)0;

        /* For k >= 1 use the recurrence */
        for (size_t j = 1; j < cur_len; ++j) {
            seq_t a_val = (j - 1 < prev_len) ? prev[j - 1] : (seq_t)0;
            seq_t b_val = (j     < prev_len) ? prev[j]     : (seq_t)0;
            seq_t c_val = (j + 1 < prev_len) ? prev[j + 1] : (seq_t)0;
            cur[j] = a_val + (seq_t)3 * b_val + c_val;
        }

        free(prev);
        prev = cur;
        prev_len = cur_len;
    }

    seq_t result = prev[k];
    free(prev);
    return result;
}

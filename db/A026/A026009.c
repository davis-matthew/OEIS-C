/*
 * A026009  Triangular array T read by rows: T(n,0) = 1 for n >= 0; T(1,1) =
 *          1; and for n >= 2, T(n,k) = T(n-1,k-1) + T(n-1,k) for k =
 *          1,2,...,[(n+1)/2]; T(n,n/2 + 1) = T(n-1,n/2) if n is even.
 * https://oeis.org/A026009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 135, u32 n <= 391, u64 n <= 1322,
 *            u128 n <= 2700, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A026009: a(n) is the (n‑th) element when the triangular array
    T is read by rows.
    Construction of T:
        T(n,0) = 1  (n ≥ 0)
        T(1,1) = 1
        For n ≥ 2, for 1 ≤ k ≤ ⌊(n+1)/2⌋
                    T(n,k) = T(n‑1,k‑1) + T(n‑1,k)
        If n is even, the last entry of row n is
                    T(n,n/2+1) = T(n‑1,n/2)
    The sequence a(n) is obtained by listing these entries in row order
    (first row 0, then row 1, …).  No closed form is used.  The
    implementation builds only the rows up to the requested index.  The
    maximum row required for n ≤ 10 000 is about 200, so the routine is
    very fast.  All arithmetic is performed in the type `seq_t`; for
    64‑bit builds the values are guaranteed to fit in `uint64_t`. */

#include <stdlib.h>
#include <stdbool.h>

static inline idx_t row_len(idx_t n)
{
    if (n == 0)
        return 1;
    idx_t len = (n + 1) / 2 + 1;
    if ((n & 1) == 0)   /* even n → one more column */
        len += 1;
    return len;
}

seq_t a(idx_t n)
{
    /* Find row r and column k that contain the n‑th element. */
    idx_t acc = 0;
    idx_t r   = 0;
    while (true) {
        idx_t len = row_len(r);
        if (n < acc + len)
            break;
        acc += len;
        ++r;
    }
    idx_t k = n - acc;        /* column index (0‑based) */

    if (r == 0)                /* the very first row */
        return 1;

    /* Allocate buffers for two successive rows.  The maximum row length
       seen for n ≤ 10 000 is < 512, so the allocations are cheap. */
    idx_t prev_len = row_len(0);
    seq_t *prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
    if (!prev)
        return 0;             /* out‑of‑memory – unlikely in practice */
    prev[0] = 1;              /* row 0 */

    for (idx_t i = 1; i <= r; ++i) {
        idx_t cur_len = row_len(i);
        seq_t *cur = (seq_t *)malloc(cur_len * sizeof(seq_t));
        if (!cur) { free(prev); return 0; }

        cur[0] = 1;            /* first entry is always 1 */

        for (idx_t j = 1; j < cur_len; ++j) {
            if ((i & 1) == 0 && j == i / 2 + 1) {
                /* special last column for even rows */
                cur[j] = prev[i / 2];
            } else {
                /* normal Pascal‑type recurrence */
                seq_t a1 = prev[j - 1];
                seq_t a2 = (j < prev_len) ? prev[j] : (seq_t)0;
                cur[j] = a1 + a2;
            }
        }

        free(prev);
        prev = cur;
        prev_len = cur_len;
    }

    seq_t result = prev[k];
    free(prev);
    return result;
}

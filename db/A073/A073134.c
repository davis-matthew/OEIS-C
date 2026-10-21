/*
 * A073134  Table by antidiagonals of T(n,k)=n*T(n,k-1)-T(n,k-2) starting with
 *          T(n,1)=1.
 * https://oeis.org/A073134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 30, i16 n <= 69, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073134:
 *  a(n) = T(n,k) where
 *    T(n,1) = 1
 *    T(n,k) = n * T(n,k-1) - T(n,k-2)   (k ≥ 2)
 *  The sequence is formed by reading the infinite table
 *  of T(n,k) along antidiagonals, starting at (1,1),
 *  then (2,1), (1,2), (1,3),(2,2),(3,1), …  */
#include <stdint.h>
#include <stddef.h>

/* Helper: compute T(n,k) for given row n and column k (k ≥ 1). */
static seq_t compute_T(seq_t n, idx_t k)
{
    if (k == 1) return 1;
    seq_t prev2 = (seq_t)0;      /* T(n,0) is implicitly 0   */
    seq_t prev1 = (seq_t)1;      /* T(n,1) = 1                */
    for (idx_t j = 2; j <= k; ++j) {
        seq_t cur = n * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;                /* T(n,k)                     */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find the diagonal number s such that (s-1)*s/2 ≥ n. */
    idx_t low = 2, high = 2;
    while ((high - 1) * high / 2 < n) high <<= 1;  /* grow high until large enough */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((mid - 1) * mid / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;                                   /* current diagonal (= sum)   */
    idx_t prev = (s - 2) * (s - 1) / 2;              /* terms before this diag    */
    idx_t offset = n - prev - 1;                     /* 0‑based within the diag  */
    idx_t row, col;
    if (s == 3) {                                   /* special ordering for the
                                                       second diagonal (sum=3) */
        row = s - 1 - offset;        /* offsets 0→2, 1→1                 */
    } else {
        row = offset + 1;            /* normal increasing row order       */
    }
    col = s - row;                       /* column = sum – row                */

    /* Evaluate T(row, col). */
    return compute_T((seq_t)row, col);
}

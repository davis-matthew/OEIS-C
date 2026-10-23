/*
 * A143908  Eigentriangle by rows, n terms of (3 * A026150) followed by
 *          A026150(n).
 * https://oeis.org/A143908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143908:
   Eigentriangle by rows, n terms of (3 * A026150) followed by A026150(n).
   Algorithm: generate the triangle row by row, storing each term and the
   sum of each row (which is A026150 of the next row).  The cache of terms
   and row-sums is expanded on demand, allowing calls in any order.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state (memoisation) */
    static seq_t *seq  = NULL;   /* flattened triangle values   */
    static size_t seq_cap = 0;   /* capacity of seq array       */
    static idx_t  seq_len  = 0;  /* number of values stored     */

    static seq_t *rowSum   = NULL;   /* R[i] = sum of row i (A026150(i+1))   */
    static size_t row_cap  = 0;       /* capacity of rowSum array           */
    /* curRow is the index of the last generated row */
    static idx_t  curRow   = 0;

    /* initialise with first row (row 0) */
    if (!seq) {
        seq_cap = 1;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(1);
        seq[0] = 1;          /* a(0) = 1 */

        row_cap = 1;
        rowSum = (seq_t*)malloc(row_cap * sizeof(seq_t));
        if (!rowSum) exit(1);
        rowSum[0] = 1;      /* sum of row 0 = 1 */

        seq_len = 1;
        curRow  = 0;
    }

    /* generate rows until the requested index is available */
    while (seq_len <= n) {
        idx_t r = curRow + 1;               /* index of the new row    */
        size_t rowLen = (size_t)(r + 1);    /* number of entries in row */

        /* grow seq array if necessary */
        if (seq_len + rowLen > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap << 1 : 1;
            while (new_cap < seq_len + rowLen) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            seq = tmp;
            seq_cap = new_cap;
        }

        /* grow rowSum array if necessary */
        if (r + 1 > row_cap) {
            size_t new_cap = row_cap ? row_cap << 1 : 1;
            while (new_cap < (size_t)(r + 1)) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(rowSum, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            rowSum = tmp;
            row_cap = new_cap;
        }

        seq_t row_sum = 0;
        idx_t pos = seq_len;       /* start position for this row */

        /* first term: 3 * A026150(0) = 3 * 1 */
        seq_t t0 = 3 * ((seq_t)1);
        seq[pos++] = t0;
        row_sum += t0;

        /* middle terms: 3 * A026150(k) for k = 1 .. r-1
           (here A026150(k) = rowSum[k-1]) */
        for (idx_t k = 1; k < r; ++k) {
            seq_t tk = 3 * rowSum[k-1];
            seq[pos++] = tk;
            row_sum += tk;
        }

        /* final term: A026150(r) = rowSum[r-1] */
        seq_t last = rowSum[r-1];
        seq[pos++] = last;
        row_sum += last;

        /* store the sum of this row for future rows */
        rowSum[r] = row_sum;

        /* update state */
        seq_len += rowLen;
        curRow   = r;
    }

    return seq[n];
}

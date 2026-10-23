/*
 * A134423  Triangle read by rows: T(n,k) is the number of paths in the first
 *          quadrant from (0,0) to (n,0) using steps U=(1,1), D=(1,-1),
 *          h=(1,0) and H=(2,0), such that the area between the x-axis and the
 *          path is k (n>=0, 0<=k<=floor(n^2/4)).
 * https://oeis.org/A134423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = row–major enumeration of the triangle T(n,k) defined by
 *            the generating function G(t,z) = 1 / (1 - z - z^2 - (t z^2)
 *            G(t,t z)). The triangle entries satisfy P[0] = 1, P[1] = 1, P[n]
 *            = P[n-1] + P[n-2] + Σ_{j=0}^{n-2} t^(j+1) P[j] P[n-2-j] (n≥2),
 *            where P[n] is the polynomial ∑_{k} T(n,k) t^k....
 * Range:     u8 n <= 63, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /*  static storage for the computed rows, their lengths and
        their cumulative entry counts.  All arrays grow as needed.  */
    static seq_t **rows      = NULL;
    static uint64_t *lens    = NULL;
    static uint64_t *cum     = NULL;
    static size_t  rows_cnt  = 0;
    static size_t  rows_cap  = 0;

    /*  initialise the first two rows on the first call  */
    if (rows_cnt == 0) {
        rows_cap = 4;
        rows = (seq_t**)malloc(rows_cap * sizeof(seq_t*));
        lens = (uint64_t*)malloc(rows_cap * sizeof(uint64_t));
        cum  = (uint64_t*)malloc(rows_cap * sizeof(uint64_t));

        /*  Row 0: T(0,0) = 1  */
        seq_t *p0 = (seq_t*)calloc(1, sizeof(seq_t));
        p0[0] = 1;
        rows[0]  = p0;
        lens[0]  = 1;
        cum[0]   = 0;                 /* cumulative entries before row 0 */

        /*  Row 1: T(1,0) = 1  */
        seq_t *p1 = (seq_t*)calloc(1, sizeof(seq_t));
        p1[0] = 1;
        if (rows_cnt + 1 >= rows_cap) {
            rows_cap <<= 1;
            rows = (seq_t**)realloc(rows,  rows_cap * sizeof(seq_t*));
            lens = (uint64_t*)realloc(lens,  rows_cap * sizeof(uint64_t));
            cum  = (uint64_t*)realloc(cum,   rows_cap * sizeof(uint64_t));
        }
        rows[1]  = p1;
        lens[1]  = 1;
        cum[1]   = cum[0] + lens[0];  /* entries before row 1 */
        rows_cnt = 2;
    }

    /*  grow rows until the requested index n fits in the current rows  */
    while ( (cum[rows_cnt-1] + lens[rows_cnt-1]) <= n ) {
        size_t next = rows_cnt;
        uint64_t len_next = 1 + (next * next) / 4;          /* floor(n^2/4)+1 */

        /*  allocate and zero‑initialise the new row  */
        seq_t *pn = (seq_t*)calloc(len_next, sizeof(seq_t));

        /*  PN ← PN + P[n-1] + P[n-2]  */
        size_t len_prev1 = lens[next-1];
        for (size_t i = 0; i < len_prev1; ++i) pn[i] += rows[next-1][i];
        size_t len_prev2 = lens[next-2];
        for (size_t i = 0; i < len_prev2; ++i) pn[i] += rows[next-2][i];

        /*  PN ← PN + Σ_{j=0}^{n-2} t^(j+1) P[j] P[n-2-j]  */
        for (size_t j = 0; j + 2 <= next; ++j) {
            seq_t *A = rows[j];
            seq_t *B = rows[next-2-j];
            size_t lenA = lens[j];
            size_t lenB = lens[next-2-j];
            size_t shift = j + 1;
            for (size_t a = 0; a < lenA; ++a) {
                for (size_t b = 0; b < lenB; ++b) {
                    pn[shift + a + b] += A[a] * B[b];
                }
            }
        }

        /*  store the new row  */
        if (rows_cnt >= rows_cap) {
            rows_cap <<= 1;
            rows = (seq_t**)realloc(rows, rows_cap * sizeof(seq_t*));
            lens = (uint64_t*)realloc(lens, rows_cap * sizeof(uint64_t));
            cum  = (uint64_t*)realloc(cum,  rows_cap * sizeof(uint64_t));
        }
        rows[rows_cnt] = pn;
        lens[rows_cnt] = len_next;
        cum[rows_cnt]  = cum[rows_cnt-1] + lens[rows_cnt-1];
        ++rows_cnt;
    }

    /*  locate the row that contains the requested flat index  */
    size_t row = 0;
    for (row = 0; row < rows_cnt; ++row) {
        uint64_t start = cum[row];
        uint64_t end   = start + lens[row];
        if (n < end) break;
    }

    /*  index within that row  */
    uint64_t idx_in_row = n - cum[row];
    return rows[row][idx_in_row];
}

/*
 * A087748  Triangle formed by reading triangle of Stirling numbers of the
 *          first kind (A048994) mod 2.
 * https://oeis.org/A087748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where T is the Stirling numbers of the first kind
 *            modulo 2. r and k are obtained by reading the triangle row by
 *            row, with r = 0,1,2,… and k=0,…,r. Recurrence: T(0,0)=1;
 *            T(n,0)=0 (n>0), T(n,n)=1; T(n,k)=T(n-1,k-1) XOR ((n-1)&1 ?
 *            T(n-1,k) : 0) (1≤k≤n-1)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute the n-th term of A087748 (0‑based index) */
seq_t a(idx_t n) {
    /* convert index n into (row r, column k) */
    idx_t r = 0;          /* current row */
    idx_t cum = 1;        /* cumulative number of items up to current row */
    while (cum <= n) {
        ++r;
        cum += (r + 1);
    }
    idx_t prev_cum = cum - (r + 1);   /* items before row r */
    idx_t k = n - prev_cum;           /* column in row r */

    /* lazily build the triangle of 0/1 values (mod 2) up to row r */
    static uint8_t **tri = NULL;          /* pointer array, tri[row][k] */
    static idx_t max_row = (idx_t)-1;     /* last row computed */

    /* initialise row 0 if needed */
    if (tri == NULL) {
        tri = (uint8_t **)malloc(sizeof(uint8_t *));
        tri[0] = (uint8_t *)malloc(1);
        tri[0][0] = 1;          /* T(0,0)=1 */
        max_row = 0;
    }

    /* compute missing rows */
    while (max_row < r) {
        idx_t next = max_row + 1;           /* new row number */
        size_t len = (size_t)(next + 1);    /* length of new row */
        uint8_t *row = (uint8_t *)malloc(len);
        if (next == 0) {
            row[0] = 1;                     /* row 0 already handled */
        } else {
            row[0] = 0;                     /* T(n,0)=0 for n>0 */
            row[next] = 1;                  /* T(n,n)=1 */
            for (idx_t kk = 1; kk < next; ++kk) {
                uint8_t a = tri[max_row][kk - 1];
                uint8_t b = ((next - 1) & 1) ? tri[max_row][kk] : 0;
                row[kk] = a ^ b;            /* XOR: addition modulo 2 */
            }
        }
        /* larger array of row pointers */
        tri = (uint8_t **)realloc(tri, (next + 1) * sizeof(uint8_t *));
        tri[next] = row;
        max_row = next;
    }

    return (seq_t)tri[r][k];
}

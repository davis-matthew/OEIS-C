/*
 * A209130  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A102756; see the Formula section.
 * https://oeis.org/A209130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = 2*T(n-1,k) + T(n-1,k-1) - T(n-2,k) + T(n-2,k-2) with
 *            T(0,0)=1, T(1,0)=1, T(1,1)=0, T(n,k)=0 when k<0 or k>n. a(n) is
 *            the linearized sequence of T(n,k) (row by row)
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **vRows = NULL;          /* vRows[r][k] = coeff of x^k in v(r,x) */
static size_t vRowsCap = 0;           /* allocated capacity for vRows[] */
static idx_t  maxRowComputed = 0;     /* largest row already built */
static seq_t *uPrev  = NULL;          /* coefficients of u(r-1,x) for next step */
static idx_t  uPrevLen = 0;

/* Ensure vRows has space for rows up to r (inclusive) and that rows 1..r
   are computed.  The triangle is built successively using the
   recurrences described in the comment. */
static void ensure_rows_upto(idx_t r)
{
    if (r <= maxRowComputed) return;

    /* Allocate or enlarge the pointer array if necessary. */
    if (!vRows || vRowsCap < (size_t)(r + 1)) {
        size_t needed = (size_t)(r + 1);
        seq_t **tmp = (seq_t**)realloc(vRows, needed * sizeof(seq_t*));
        if (!tmp) { /* allocation failure – return early (sequence incomplete) */
            return;
        }
        /* If we increased the capacity, initialise new slots to NULL. */
        for (size_t i = vRowsCap; i < needed; ++i) tmp[i] = NULL;
        vRows = tmp;
        vRowsCap = needed;
    }

    /* Initialise the very first row if we have not done so yet. */
    if (maxRowComputed == 0) {
        /* u(1,x)=1, v(1,x)=1 */
        uPrev = (seq_t*)malloc(sizeof(seq_t));
        if (!uPrev) return;
        uPrevLen = 1;
        uPrev[0] = (seq_t)1;

        vRows[1] = (seq_t*)malloc(sizeof(seq_t));
        if (!vRows[1]) return;
        vRows[1][0] = (seq_t)1;

        maxRowComputed = 1;
    }

    /* Build new rows up to r. */
    for (idx_t cur = maxRowComputed + 1; cur <= r; ++cur) {
        idx_t prevLen = uPrevLen;      /* length of previous row (cur-1)   */
        idx_t curLen  = cur;           /* length of current row            */
        seq_t *vPrev = vRows[cur - 1]; /* v(n-1,x) coefficients           */

        /* Allocate arrays for u_cur and v_cur. */
        seq_t *uCur = (seq_t*)malloc(curLen * sizeof(seq_t));
        seq_t *vCur = (seq_t*)malloc(curLen * sizeof(seq_t));
        if (!uCur || !vCur) return;

        for (idx_t k = 0; k < curLen; ++k) {
            /* u_cur[k] = u_prev[k] + v_prev[k] + v_prev[k-1] (if exists) */
            seq_t valU = 0;
            if (k < prevLen) valU += uPrev[k];
            if (k < prevLen) valU += vPrev[k];
            if (k > 0 && (k - 1) < prevLen) valU += vPrev[k - 1];
            uCur[k] = valU;

            /* v_cur[k] = u_prev[k-1] + v_prev[k] + v_prev[k-1] (if exists) */
            seq_t valV = 0;
            if (k > 0 && (k - 1) < prevLen) valV += uPrev[k - 1];
            if (k < prevLen) valV += vPrev[k];
            if (k > 0 && (k - 1) < prevLen) valV += vPrev[k - 1];
            vCur[k] = valV;
        }

        /* Store current v row; free previous u; move uPrev pointer. */
        vRows[cur] = vCur;
        free(uPrev);
        uPrev = uCur;
        uPrevLen = curLen;

        maxRowComputed = cur;
    }
}

/* Map linear index n (≥1) to row r and column k (0‑based). */
static void index_to_rowcol(idx_t n, idx_t *row, idx_t *col)
{
    /* Find smallest r with r*(r+1)/2 ≥ n. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) r++;
    *row = r;
    idx_t prevSum = (r - 1) * r / 2;
    *col = n - prevSum - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* below offset – undefined */
    idx_t row, col;
    index_to_rowcol(n, &row, &col);
    ensure_rows_upto(row);
    if (row <= maxRowComputed && (size_t)col < (size_t)row) {
        return vRows[row][col];
    }
    return (seq_t)0;   /* safety – should not happen */
}

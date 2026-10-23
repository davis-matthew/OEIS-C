/*
 * A008277  Triangle of Stirling numbers of the second kind, S2(n,k), n >= 1,
 *          1 <= k <= n.
 * https://oeis.org/A008277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, nice, core, changed
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 140, u64 n <= 358,
 *            u128 n <= 957, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A008277:  Triangle of Stirling numbers of the second kind.
 *  a(1) = 1
 *
 *  For a given index n (1‑based) a(n) is the element S2(r,k) where
 *  r is the unique integer with r(r+1)/2 >= n, and
 *  k = n - (r-1)r/2  (1 <= k <= r).
 *
 *  The function builds a dynamic table of Stirling numbers on demand
 *  and re‑uses it for subsequent calls, guaranteeing correct results for
 *  any n >= 1, independent of call order.
 */

#include <stdlib.h>

/* ------------------------------------------------------------ */
/*  Helper: integer square‑root for unsigned 64‑bit numbers      */
/* ------------------------------------------------------------ */
static inline idx_t int_sqrt(idx_t x)
{
    /* returns floor(sqrt(x)) */
    if (x == 0 || x == 1) return x;
    idx_t lo = 0, hi = x >> 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * mid == x) return mid;
        if ((unsigned __int128)mid * mid < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                     */
/* ------------------------------------------------------------ */
static seq_t **s2_table = NULL;   /* table of rows; index 0 unused */
static idx_t   max_row   = 0;     /* highest row currently computed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* index 0 is out of range */

    /* --- Find the row r such that r(r+1)/2 >= n ----------------- */
    idx_t low  = 1;
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t r = low;          /* row number */

    /* --- Compute columns needed for this row --------------------- */
    if (max_row < r) {
        /* Allocate or grow the outer table */
        seq_t **new_table = (seq_t**)realloc(s2_table, (r + 1) * sizeof(seq_t*));
        s2_table = new_table;

        /* Build missing rows one by one */
        for (idx_t i = max_row + 1; i <= r; ++i) {
            s2_table[i] = (seq_t*)calloc((size_t)i + 1, sizeof(seq_t));
            /* S2(i,1) = S2(i,i) = 1 */
            s2_table[i][1] = 1;
            s2_table[i][i] = 1;

            for (idx_t j = 2; j < i; ++j) {
                /* Recurrence: S2(i,j) = j*S2(i-1,j) + S2(i-1,j-1) */
                s2_table[i][j] = s2_table[i-1][j] * (seq_t)j
                                + s2_table[i-1][j-1];
            }
        }
        max_row = r;
    }

    /* --- Find column k within the row ------------------------ */
    idx_t prev_count = (unsigned __int128)(r - 1) * r / 2; /* elements before row r */
    idx_t k = n - prev_count;  /* 1‑based column index */

    return s2_table[r][k];
}

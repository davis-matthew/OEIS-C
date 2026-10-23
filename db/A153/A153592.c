/*
 * A153592  Triangle read by rows: T(n,k) = T(n-1,k-1) +T(n-1,k)
 *          +n*(n-1)*T(n-2,k-1) for n>4 and 1<=k<=n.
 * https://oeis.org/A153592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A153592: Triangle read by rows
   T(n,k) = T(n-1,k-1) + T(n-1,k) + n*(n-1)*T(n-2,k-1) (n>4)
   Initial values:
      T(1,1)=2
      T(2,1)=T(2,2)=3
      T(3,1)=T(3,3)=2 , T(3,2)=20
      T(4,1)=T(4,4)=2 , T(4,2)=T(4,3)=58
   The OEIS sequence a(n) is the flattened triangle read row-wise.
*/
#include <stdlib.h>

static seq_t **triangle = NULL;    /* array of row pointers, 1‑indexed */
static idx_t max_row    = 0;        /* highest computed row */
static size_t cap_rows  = 0;        /* capacity of triangle array */

/* Find the row that contains the n‑th element (1‑based) */
static idx_t row_for_idx(idx_t n)
{
    if (n < 1) return 0;
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) high = mid;
        else          low  = mid + 1;
    }
    return low;
}

/* Ensure rows 1..needed are computed */
static void ensure_rows(idx_t needed)
{
    if (needed <= max_row) return;

    /* increase storage for row pointers if necessary */
    if (needed + 1 > cap_rows) {
        size_t new_cap = cap_rows ? cap_rows * 2 : 1;
        while (new_cap <= (size_t)needed + 1) new_cap <<= 1;
        triangle = (seq_t**)realloc(triangle, new_cap * sizeof(seq_t *));
        for (size_t i = cap_rows; i < new_cap; ++i) triangle[i] = NULL;
        cap_rows = new_cap;
    }

    for (idx_t i = max_row + 1; i <= needed; ++i) {
        triangle[i] = (seq_t*)malloc(sizeof(seq_t) * (i + 2));
        if (!triangle[i]) return;           /* allocation failed – ignore */
        triangle[i][0]      = 0;           /* unused boundary 0 */
        triangle[i][i + 1]  = 0;           /* unused boundary i+1 */

        if (i == 1) {
            triangle[i][1] = 2;
        } else if (i == 2) {
            triangle[i][1] = triangle[i][2] = 3;
        } else if (i == 3) {
            triangle[i][1] = triangle[i][3] = 2;
            triangle[i][2] = 20;
        } else if (i == 4) {
            triangle[i][1] = triangle[i][4] = 2;
            triangle[i][2] = triangle[i][3] = 58;
        } else {
            seq_t factor = (seq_t)i * (seq_t)(i - 1);  /* n*(n-1) */
            for (idx_t j = 1; j <= i; ++j) {
                seq_t a = triangle[i-1][j-1];
                seq_t b = triangle[i-1][j];
                seq_t c = (j-1 <= i-2) ? factor * triangle[i-2][j-1] : 0;
                triangle[i][j] = a + b + c;
            }
        }
    }
    max_row = needed;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t r = row_for_idx(n);
    idx_t prev_sum = (r - 1) * r / 2;      /* elements before row r */
    idx_t k = n - prev_sum;                /* column within row r (1‑based) */
    ensure_rows(r);
    return triangle[r][k];
}

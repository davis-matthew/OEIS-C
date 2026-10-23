/*
 * A132883  Triangle read by rows: T(n,k) is the number of paths in the first
 *          quadrant from (0,0) to (n,0), consisting of steps U=(1,1),
 *          D=(1,-1), h=(1,0) and H=(2,0), having k U steps (0 <= k <=
 *          floor(n/2)).
 * https://oeis.org/A132883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence derived from the functional equation
    G(t,z) = 1 + z G + z^2 G + t z^2 G^2 for the triangle
    T(n,k) (OEIS A132883).  
    For n = 0,1 :  T(0,0)=1 , T(1,0)=1 .
    For n ≥ 2 and 0 ≤ k ≤ ⌊n/2⌋ :

        T(n,0)   = T(n‑1,0) + T(n‑2,0)

        T(n,k)   = T(n‑1,k) + T(n‑2,k) +
                   Σ_{i=0}^{n‑2} Σ_{j=0}^{k‑1} T(i,j) * T(n‑2‑i, k‑1‑j)

    The function a(idx_t n) returns the flattening of this triangle,
    i.e. the list of all T(n,k) read by rows.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

struct Row {
    idx_t len;     /* number of elements in this row            */
    seq_t *data;   /* pointer to the row's contents             */
};

/* Maximum number of rows that may be required for n ≤ 10000
   (rows grow roughly like sqrt(2*n)).  The value 2000 is more than
   enough for any such n. */
#define MAX_ROWS 2000

/* Static state: rows, flattened sequence, etc. */
static struct Row rows[MAX_ROWS];
static size_t rows_used = 0;
static seq_t *flat = NULL;
static size_t flat_cap = 0;
static size_t flat_used = 0;
static int initialized = 0;

static void ensure_flat_capacity(size_t need)
{
    while (flat_cap < need) {
        flat_cap = flat_cap ? flat_cap * 2 : 128;
        flat = (seq_t *)realloc(flat, flat_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    /* Initialization on first call: compute rows 0 and 1 and the
       initial part of the flattened array. */
    if (!initialized) {
        /* row 0 : T(0,0)=1 */
        rows[0].len = 1;
        rows[0].data = (seq_t *)malloc(sizeof(seq_t) * 1);
        rows[0].data[0] = (seq_t)1;

        /* row 1 : T(1,0)=1 */
        rows[1].len = 1;
        rows[1].data = (seq_t *)malloc(sizeof(seq_t) * 1);
        rows[1].data[0] = (seq_t)1;

        /* flattened array */
        flat_cap = 128;
        flat = (seq_t *)malloc(sizeof(seq_t) * flat_cap);
        flat[0] = (seq_t)1;          /* row 0 */
        flat[1] = (seq_t)1;          /* row 1 */
        flat_used = 2;

        rows_used = 2;
        initialized = 1;
    }

    /* Expand the triangle until we have at least (n+1) entries in
       the flattened sequence. */
    while (flat_used <= n) {
        idx_t r = rows_used;     /* the index of the new row to compute */

        /* Size of the new row */
        size_t len = (size_t)(r / 2) + 1;
        seq_t *cur = (seq_t *)malloc(sizeof(seq_t) * len);

        /* k = 0 case: Fibonacci recurrence */
        cur[0] = rows[r-1].data[0] + rows[r-2].data[0];

        /* k > 0 cases */
        for (size_t k = 1; k < len; ++k) {
            seq_t val = 0;

            if (k < rows[r-1].len)
                val += rows[r-1].data[k];
            if (k < rows[r-2].len)
                val += rows[r-2].data[k];

            /* Convolution part: Σ_{i=0}^{r-2} Σ_{j=0}^{k-1}
               T(i,j) * T(r-2-i, k-1-j) */
            for (size_t i = 0; i <= (size_t)r-2; ++i) {
                size_t i_len = rows[i].len;
                size_t jmax = k-1;
                size_t jlimit = rows[(size_t)r-2-i].len;

                for (size_t j = 0; j <= jmax && j < i_len; ++j) {
                    size_t j2 = k-1-j;
                    if (j2 >= jlimit) continue;
                    val += rows[i].data[j] * rows[(size_t)r-2-i].data[j2];
                }
            }
            cur[k] = val;
        }

        /* Store the new row */
        rows[r].len = len;
        rows[r].data = cur;
        rows_used++;

        /* Append the new row to the flattened array */
        ensure_flat_capacity(flat_used + len);
        for (size_t k = 0; k < len; ++k)
            flat[flat_used++] = cur[k];
    }

    return flat[n];
}

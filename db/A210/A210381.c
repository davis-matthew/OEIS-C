/*
 * A210381  Triangle by rows, derived from the beheaded Pascal's triangle,
 *          A074909.
 * https://oeis.org/A210381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is given by flattening the triangle T(n,k) defined by the *
 *            recurrence * * T(n,k) = T(n‑1,k) + 2*T(n‑1,k‑1) – T(n‑2,k‑1) –
 *            T(n‑2,k‑2) * * with initial values * * T(0,0) = 1, * T(1,0) = 0,
 *            T(1,1) = 2, * T(2,0) = 0, T(2,1) = 1, T(2,2) = 3, * * and T(n,k)
 *            = 0 for k<0 or k>n. * The sequence list...
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t      max_row   = (idx_t)-1;   /* number of rows computed – 1 if uninitialised */
static idx_t     *base      = NULL;       /* base[row] = offset of first element of row in flat array */
static size_t    base_cap   = 0;           /* capacity of base array                 */
static seq_t    *triangle   = NULL;       /* flat storage of triangle entries       */
static size_t    flat_cap   = 0;           /* capacity of triangle array             */
static size_t    seq_len    = 0;           /* number of a(n) values stored           */

static seq_t get_T(idx_t r, idx_t c)
{
    if (c > r) return (seq_t)0;
    return triangle[ base[r] + c ];
}

seq_t a(idx_t n)
{
    /* Initialise the first row on first call */
    if (max_row == (idx_t)-1) {
        base_cap = 1;
        base = (idx_t*)malloc(base_cap * sizeof(idx_t));
        base[0] = 0;                          /* offset of row 0 */

        flat_cap = 1;
        triangle = (seq_t*)malloc(flat_cap * sizeof(seq_t));
        triangle[0] = (seq_t)1;                /* T(0,0) */

        seq_len = 1;                          /* number of values stored */
        max_row = 0;                           /* rows computed so far */
    }

    /* Extend the triangle until we have computed a(n) */
    while (seq_len <= (size_t)n) {
        idx_t  new_row       = max_row + 1;      /* next row to compute   */
        idx_t  new_row_len   = new_row + 1;      /* number of entries in new row */

        /* Ensure capacity for base array */
        if ((size_t)(new_row + 1) > base_cap) {
            size_t new_cap = base_cap ? base_cap * 2 : 1;
            if ((size_t)(new_row + 1) > new_cap) new_cap = (size_t)(new_row + 1);
            idx_t *tmp = (idx_t*)realloc(base, new_cap * sizeof(idx_t));
            base = tmp;
            base_cap = new_cap;
        }
        base[new_row] = seq_len;                /* starting offset of new row */

        /* Ensure capacity for triangle entries */
        size_t needed = seq_len + new_row_len;
        if (needed > flat_cap) {
            size_t new_cap = flat_cap ? flat_cap * 2 : 1;
            if (needed > new_cap) new_cap = needed;
            seq_t *tmp = (seq_t*)realloc(triangle, new_cap * sizeof(seq_t));
            triangle = tmp;
            flat_cap = new_cap;
        }

        /* Compute the entries of the new row */
        for (idx_t k = 0; k <= new_row; ++k) {
            seq_t val;
            if (new_row == 0) {
                val = (seq_t)1;                                     /* T(0,0) */
            } else if (new_row == 1) {
                val = (k == 0 ? (seq_t)0 : (seq_t)2);
            } else if (new_row == 2) {
                if (k == 0)      val = (seq_t)0;
                else if (k == 1) val = (seq_t)1;
                else             val = (seq_t)3;
            } else {
                /* Recurrence: T(n,k) = T(n‑1,k) + 2*T(n‑1,k‑1) – T(n‑2,k‑1) – T(n‑2,k‑2). */
                seq_t t1 = (k <= new_row-1) ? get_T(new_row-1, k) : (seq_t)0;
                seq_t t2 = (k >= 1) ? get_T(new_row-1, k-1) : (seq_t)0;
                seq_t t3 = (k >= 1) ? get_T(new_row-2, k-1) : (seq_t)0;
                seq_t t4 = (k >= 2) ? get_T(new_row-2, k-2) : (seq_t)0;
                val = t1 + (t2 << 1) - t3 - t4;
            }
            triangle[seq_len++] = val;
        }
        max_row = new_row;
    }

    /* Return the requested value */
    return triangle[n];
}

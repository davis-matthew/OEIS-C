/*
 * A179045  Triangle T(n,k), 1<=k<=n, read by rows, related to A033485.
 * https://oeis.org/A179045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the triangle read by rows, where
 *            T(n,k) satisfies T(n,k) = T(n-1,k) + T([n/2],k) (1) T(n,n) = 1
 *            T(n,k) = 0 if k>n
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* A flat triangular array that stores T[n][k] with 1‑based indices.
   offset for row r (1‑based) is (r‑1)*r/2.                 */
static seq_t *flat  = NULL;          /* storage for all T(n,k) up to max_row */
static idx_t  flat_size = 0;         /* number of seq_t elements allocated */
static idx_t  max_row = 0;           /* largest row that has been computed */

/* Compute the starting index of row r in the flat array.             */
static inline idx_t offset_row(idx_t r) {
    return (r - 1) * r / 2;
}

/* Grow the flat array to be able to store all rows up to up_to.       */
static void ensure_flat(idx_t up_to) {
    if (up_to <= max_row) return;

    /* Number of entries needed after we add rows up to up_to.          */
    idx_t new_size = offset_row(up_to) + up_to;

    if (new_size > flat_size) {
        flat = (seq_t *)realloc(flat,
                                (size_t)new_size * sizeof(seq_t));
        /* If realloc fails, the program will abort;  this is fine.  */
        flat_size = new_size;
    }

    /* Compute new rows.  All entries will be written.                  */
    for (idx_t r = max_row + 1; r <= up_to; ++r) {
        idx_t half = r / 2;
        idx_t offset_r = offset_row(r);
        idx_t offset_r_minus = offset_row(r - 1);
        idx_t offset_half = offset_row(half);

        for (idx_t k = 1; k <= r; ++k) {
            seq_t val;
            if (k == r) {
                /* Boundary condition */
                val = (seq_t)1;
            } else {
                seq_t sum = flat[offset_r_minus + (k - 1)];
                if (k <= half) {
                    sum += flat[offset_half + (k - 1)];
                }
                val = sum;
            }
            flat[offset_r + (k - 1)] = val;
        }
    }
    max_row = up_to;
}

/* External interface: compute the n-th term of the sequence (n ≥ 1).   */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* sequence is only defined for n ≥ 1 */

    /* Find row r such that r(r+1)/2 ≥ n.                            */
    idx_t r = 1;
    while (((unsigned __int128)r * (r + 1)) / 2 < n) ++r;

    /* Column within that row.                                      */
    idx_t c = n - offset_row(r);   /* c is 1‑based */

    ensure_flat(r);
    return flat[offset_row(r) + (c - 1)];
}

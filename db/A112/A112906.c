/*
 * A112906  A skew generalized Pascal triangle.
 * https://oeis.org/A112906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 63, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(r, k) where r is the unique integer with
 *                r(r+1)/2 ≤ n < (r+1)(r+2)/2,
 *              and k = n – r(r+1)/2.
 *
 * T satisfies:
 *   T(0,0) = 1,
 *   T(n,0) = 0   (n > 0),
 *   T(n,k) = 3·T(n-1,k-1) + T(n-2,k-1) + T(n-2,k-2).
 *
 * The function builds the triangle up to the required row only once
 * and caches it in a static array.  Subsequent calls use the cached
 * values.  All arithmetic is performed with the type seq_t, so the
 * implementation works correctly for 8, 16, 32, 64 or 128‑bit
 * integer types as well as arbitrary‑precision integers.
 */

#include <stdlib.h>

/* Cached triangle entries */
static seq_t *triangle = NULL;        /* linear array of T(r,k) values */
static idx_t  triangle_size = 0;      /* number of entries currently cached */

/*
 * Compute the skew Pascal triangle up to (and including) all indices ≤ n.
 * The total number of entries required for rows 0 … rmax is
 *   (rmax+1)(rmax+2)/2
 * so we allocate that many seq_t elements.  The triangle is stored
 * in row‑major order:  index = r(r+1)/2 + k.
 */
static void
compute_triangle_upto(idx_t n)
{
    if (n < triangle_size) {
        /* already computed up to this index */
        return;
    }

    /* Find the largest r such that r(r+1)/2 ≤ n. */
    idx_t rmax = 0;
    while ((rmax + 1) * (rmax + 2) / 2 <= n) {
        ++rmax;
    }

    /* Total number of entries up to row rmax (inclusive). */
    idx_t new_size = (rmax + 1) * (rmax + 2) / 2;

    triangle = (seq_t *)realloc(triangle, new_size * sizeof(seq_t));
    if (triangle == NULL) {
        /* Out of memory; abort (the build environment is expected
         * to provide enough memory for this small cache). */
        abort();
    }

    /* Build the triangle row by row.  We assume that any entry
     * accessed out of bounds (i.e. T(n,k) with n<0 or k<0) is
     * implicitly zero. */
    for (idx_t r = 0; r <= rmax; ++r) {
        for (idx_t k = 0; k <= r; ++k) {
            idx_t pos = r * (r + 1) / 2 + k;
            seq_t val;

            if (r == 0 && k == 0) {
                /* Base case */
                val = (seq_t)1;
            } else if (k == 0) {
                /* First column except the very first element */
                val = (seq_t)0;
            } else {
                val = (seq_t)0;

                /* Term involving T(r-1, k-1) */
                idx_t pos1 = ((r - 1) * r) / 2 + (k - 1);
                val += ((seq_t)3) * triangle[pos1];

                if (r >= 2) {
                    /* Term involving T(r-2, k-1) */
                    idx_t pos2 = ((r - 2) * (r - 1)) / 2 + (k - 1);
                    val += triangle[pos2];

                    if (k >= 2) {
                        /* Term involving T(r-2, k-2) */
                        idx_t pos3 = ((r - 2) * (r - 1)) / 2 + (k - 2);
                        val += triangle[pos3];
                    }
                }
            }

            triangle[pos] = val;
        }
    }

    triangle_size = new_size;
}

/* PUBLIC FUNCTION ------------------------------------------------------ */

seq_t
a(idx_t n)
{
    compute_triangle_upto(n);
    return triangle[n];
}

/*
 * A008957  Triangle of central factorial numbers T(2*n,2*n-2*k), k >= 0, n >=
 *          1 (in Riordan's notation).
 * https://oeis.org/A008957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, tabl
 * Formula:   T(n,k) = T(n-1,k-1) + k^2 * T(n-1,k), with T(n,1)=T(n,n)=1. Into
 *            this triangle one writes the rows in order, and in each row the
 *            elements are placed with column index decreasing from n to 1.
 *            a(n) is the n-th element of this row‑wise enumeration
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 72, u64 n <= 163,
 *            u128 n <= 423, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#  include <new>
#endif

/* Cached triangle: rows are 1‑based; each row contains indices 1..row
   (index 0 is unused). */
static seq_t **tri = NULL;
static idx_t tri_max = 0;

/* Ensure the triangle is computed up to the required row `need`. */
static void ensure_triangle(idx_t need)
{
    if (need <= tri_max) return;

    /* Resize the array of row pointers. */
    tri = (seq_t**)realloc(tri, (need + 1) * sizeof(seq_t*));

    for (idx_t r = tri_max + 1; r <= need; ++r) {
        /* Allocate the actual row.  Use `new` in C++ and `malloc` in C. */
        seq_t *row = NULL;
#ifdef __cplusplus
        row = new seq_t[r + 1];
#else
        row = malloc((r + 1) * sizeof(seq_t));
#endif
        /* Base cases */
        row[1] = (seq_t)1;
        row[r] = (seq_t)1;

        /* Fill interior entries using the recurrence. */
        for (idx_t k = 2; k <= r - 1; ++k) {
            seq_t left   = tri[r - 1][k - 1];
            seq_t mid    = tri[r - 1][k];
            idx_t k2 = k * k;                       /* integer square */
            seq_t prod = mid * (seq_t)k2;
            row[k] = left + prod;
        }
        tri[r] = row;
    }
    tri_max = need;
}

/* Return the n-th term of A008957. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* Find the row r such that 1 + 2 + ... + r >= n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Ensure all rows up to r are present. */
    ensure_triangle(r);

    /* Position inside the row (0‑based) */
    idx_t prev_total = (r - 1) * r / 2;
    idx_t offset = n - prev_total - 1;      /* 0 ≤ offset < r */
    /* Columns in the sequence go from k = r down to 1. */
    idx_t k = r - offset;

    return tri[r][k];
}

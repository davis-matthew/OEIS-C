/*
 * A208532  Mirror image of triangle in A125185; unsigned version of A120058.
 * https://oeis.org/A208532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is obtained by flattening the lower triangular array T(n,k)
 *            row‑wise. * The array satisfies: * T(0,0)=1, T(1,1)=1 * T(n,k)=0
 *            when k<0 or k>n * for n≥2 and 0≤k≤n * T(n,k) = 2*T(n-1,k) +
 *            2*T(n-1,k-1) - T(n-2,k) - 2*T(n-2,k-1). * Row sums are 3^n and
 *            can be used as a test
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of the flattened triangle */
static seq_t  *flat = NULL;          /* contiguous storage of all rows */
static size_t flat_capacity = 0;     /* allocated size of 'flat'              */
static size_t max_row_computed = (size_t)-1;  /* rows 0..max_row_computed are stored; sentinel */

/* Grow the flat array to at least 'needed' elements. */
static void grow_flat(size_t needed)
{
    if (needed <= flat_capacity)
        return;
    size_t new_cap = flat_capacity ? flat_capacity * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(flat, new_cap * sizeof(seq_t));
    if (!tmp) /* allocation failure: terminate */
        abort();
    flat = tmp;
    flat_capacity = new_cap;
}

/* Generate row 'r' (r ≥ 0) and store its values in 'flat'. */
static void generate_row(idx_t r)
{
    size_t base = (size_t)r * (r + 1) / 2;
    /* ensure room for row r */
    grow_flat(base + r + 1);

    if (r == 0) {
        flat[base] = (seq_t)1;
    } else if (r == 1) {
        /* row 1: k=0 is 2, k=1 is 1 */
        flat[base]     = (seq_t)2;  /* T(1,0) */
        flat[base + 1] = (seq_t)1;  /* T(1,1) */
    } else {
        /* Indices of previous two rows */
        size_t prev_offset   = (size_t)(r - 1) * r / 2;          /* start of row r-1 */
        size_t prev2_offset  = (size_t)(r - 2) * (r - 1) / 2;    /* start of row r-2 */

        for (idx_t k = 0; k <= r; ++k) {
            seq_t a = 0, b = 0, c = 0, d = 0;

            /* T(r-1,k) */
            if (k <= r - 1)
                a = flat[prev_offset + k];

            /* T(r-1,k-1) */
            if (k >= 1)
                b = flat[prev_offset + k - 1];

            /* T(r-2,k) */
            if (k <= r - 2)
                c = flat[prev2_offset + k];

            /* T(r-2,k-1) */
            if (k >= 1 && k - 1 <= r - 2)
                d = flat[prev2_offset + k - 1];

            seq_t val = (seq_t)2 * a + (seq_t)2 * b - c - (seq_t)2 * d;
            flat[base + k] = val;
        }
    }
    max_row_computed = (size_t)r;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* Locate the row r and column k such that n = r*(r+1)/2 + k */
    idx_t r = 0;
    /* find r such that start(r+1) > n  */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* Generate all rows up to r if needed */
    for (size_t row = (max_row_computed == (size_t)-1) ? 0 : max_row_computed + 1;
         row <= (size_t)r; ++row)
        generate_row((idx_t)row);

    size_t flat_index = (size_t)r * (r + 1) / 2 + k;
    return flat[flat_index];
}

/*
 * A135856  A007318 * a bidiagonal matrix with all 1's in the main diagonal
 *          and all 4's in the subdiagonal.
 * https://oeis.org/A135856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A135856: the product of the binomial transform of a bidiagonal matrix
   with 1's on the main diagonal and 4's on the subdiagonal, taken
   row‑wise.  For row i (i ≥ 1) and column j (1 ≤ j ≤ i) the triangle
   entry is B(i,j) = C(i‑1,j‑1) + 4·C(i‑1,j) for j < i, and B(i,i) = 1.
   The sequence lists the entries in row order: a(1) = 1, a(2) = 5, a(3) = 1, … */

#include <stdlib.h>

/* static storage for memoization */
static seq_t *cache = NULL;          /* cached sequence values, 0‑based */
static size_t cache_cap = 0;         /* capacity of cache in elements   */
static size_t cache_size = 0;        /* number of terms already stored  */

/* row of binomial coefficients for the previous row (C(i‑1, j)) */
static seq_t *prev_row = NULL;      /* previous row content           */
static size_t prev_len = 0;          /* length of prev_row             */
static idx_t  current_row = 0;       /* number of rows computed so far */

/* Ensure cache can hold at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc((void*)cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);            /* unlikely; abort on OOM */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for n ≥ 1.  The function is thread‑unsafe but satisfies
   the specified requirements. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0 */

    /* extend the cached sequence up to n */
    while (cache_size < (size_t)n)
    {
        /* compute next row of the triangle */
        ++current_row;                  /* new row index i = current_row */
        idx_t i = current_row;          /* number of elements in this row */

        /* allocate space for binomial coefficients C(i‑1, j), j=0..i‑1 */
        size_t new_len = (size_t)i;
        seq_t *row = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!row) exit(1);              /* out of memory */

        /* first coefficient is always 1 */
        row[0] = (seq_t)1;

        if (i > 1) {
            /* build the i‑th row from the previous row (Pascal rule) */
            for (size_t j = 1; j + 1 < new_len; ++j)
                row[j] = prev_row[j - 1] + prev_row[j];
            row[new_len - 1] = (seq_t)1;
        }

        /* generate the triangle entries for this row and cache them */
        for (size_t j = 1; j <= new_len; ++j)
        {
            seq_t val;
            if (j == new_len)            /* last element of the row */
                val = (seq_t)1;
            else
                val = row[j - 1] + (seq_t)4 * row[j];

            grow_cache(cache_size + 1);
            cache[cache_size++] = val;
        }

        /* free the previous row and prepare for the next iteration */
        free(prev_row);
        prev_row = row;
        prev_len = new_len;
    }

    return cache[(size_t)(n - 1)];
}

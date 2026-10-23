/*
 * A142978  Table of figurate numbers for the n-dimensional cross polytopes.
 * https://oeis.org/A142978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 112, u32 n <= 391, u64 n <= 1455,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Table of figurate numbers for the n‑dimensional cross polytopes (OEIS A142978).  
 * a(n) is obtained by reading the triangle T(n,k) along its anti‑diagonals.
 * Recurrence for T:
 *   T(1,k) = k
 *   T(n,1) = 1
 *   T(n,k) = T(n,k-1) + T(n-1,k-1) + T(n-1,k)   (n,k > 1)
 *
 * For a given index n the corresponding coordinates (i,j) satisfy:
 *   i + j = s   where s is the smallest integer with (s-1)*s/2 ≥ n
 *   p           = n - (s-2)*(s-1)/2                         (1‑based in the diagonal)
 *   i           = s - p
 *   j           = p
 *
 * The function builds and caches the triangle up to the required depth.
 * It works for any seq_t and idx_t provided by the build. */

#include <stdlib.h>
#include <math.h>

/* forward declaration of helpers */
static void ensure_table(size_t dim);
static void idx_to_ij(idx_t n, idx_t *i, idx_t *j);

/* global cache for the triangle */
static seq_t *table = NULL;
static size_t width = 0;          /* width of the square table (dim+1) */
static size_t max_dim = 0;

/* cache helper – recompute the triangle up to dimension 'dim' (1‑based) */
static void ensure_table(size_t dim)
{
    if (dim <= max_dim)
        return;

    /* allocate new table of size (dim+1) × (dim+1) */
    size_t w = dim + 1;
    seq_t *new_table = (seq_t *)calloc(w * w, sizeof(seq_t));
    if (!new_table)
        return;   /* out of memory – caller will fail gracefully */

    /* fill the triangle */
    for (size_t k = 1; k <= dim; ++k)
        new_table[1 * w + k] = (seq_t)k;   /* T(1,k) = k */

    for (size_t n = 2; n <= dim; ++n) {
        new_table[n * w + 1] = (seq_t)1;   /* T(n,1) = 1 */
        for (size_t k = 2; k <= dim; ++k) {
            seq_t a = new_table[n * w + (k - 1)];
            seq_t b = new_table[(n - 1) * w + (k - 1)];
            seq_t c = new_table[(n - 1) * w + k];
            new_table[n * w + k] = a + b + c;
        }
    }

    free(table);           /* discard old cache */
    table = new_table;     /* store new cache */
    width = w;
    max_dim = dim;
}

/* convert sequence index n to triangle coordinates (i,j) following the anti‑diagonals */
static void idx_to_ij(idx_t n, idx_t *i, idx_t *j)
{
    /* find smallest s such that (s-1)*s/2 >= n */
    double disc = sqrt(8.0 * (double)n - 7.0);
    idx_t s = (idx_t)((disc - 1.0) / 2.0) + 1;
    while (((idx_t)(s - 1) * s) / 2 < n)
        ++s;

    idx_t prev = ((idx_t)(s - 2) * (s - 1)) / 2;   /* elements before diagonal s */
    idx_t p = n - prev;                           /* position inside the diagonal (1‑based) */

    *i = s - p;
    *j = p;
}

/* public function – compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* offset is 1 – n=0 not defined */

    idx_t i, j;
    idx_to_ij(n, &i, &j);

    size_t needed = (size_t)((i > j) ? i : j);   /* table must cover both i and j */
    ensure_table(needed);

    return table[i * width + j];
}

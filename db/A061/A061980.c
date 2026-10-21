/*
 * A061980  Square array A(n,k) = A(n-1,k) + A(n-1, floor(k/2)) + A(n-1,
 *          floor(k/3)), with A(0,0) = 1, read by antidiagonals.
 * https://oeis.org/A061980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = A(n-1,k) + A(n-1, floor(k/2)) + A(n-1, floor(k/3)),
 *            A(0,0) = 1; read by antidiagonals for a(n)
 * Range:     u8 n <= 26, u16 n <= 76, u32 n <= 251, u64 n <= 901,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Memoisation for the 2‑D array A(n,k). */
static size_t cache_dim = 0;          /* current dimension (max i or j + 1)   */
static seq_t *cache      = NULL;      /* flattened 2‑D array [i*cache_dim + j] */
static unsigned char *computed = NULL;/* flag array, 1 if entry computed */

/* Grow the internal cache to at least (needed+1) × (needed+1) size. */
static void ensure_cache_dim(size_t needed)
{
    if (needed + 1 <= cache_dim)
        return;                       /* already big enough */

    size_t new_dim = cache_dim > 0 ? cache_dim : 1;
    while (new_dim <= needed + 1)
        new_dim <<= 1;                  /* make it a power of two for speed */

    size_t new_size = new_dim * new_dim;

    seq_t *new_cache  = (seq_t *)calloc(new_size, sizeof(seq_t));
    unsigned char *new_comp = (unsigned char *)calloc(new_size, sizeof(unsigned char));

    /* copy old data into the larger array */
    for (size_t i = 0; i < cache_dim; ++i) {
        for (size_t j = 0; j < cache_dim; ++j) {
            size_t old_idx = i * cache_dim + j;
            size_t new_idx = i * new_dim + j;
            new_cache[new_idx] = cache[old_idx];
            new_comp[new_idx] = computed[old_idx];
        }
    }

    free(cache);
    free(computed);

    cache      = new_cache;
    computed   = new_comp;
    cache_dim  = new_dim;
}

/* Recursive definition of A(i,j) with memoisation. */
static seq_t compute_A(size_t i, size_t j)
{
    if (i == 0) {
        /* A(0,0)=1, A(0,j>0)=0 */
        return (j == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* Ensure the cache can hold this indices pair. */
    ensure_cache_dim((i > j) ? i : j);

    size_t idx = i * cache_dim + j;
    if (computed[idx])
        return cache[idx];

    seq_t val = compute_A(i - 1, j)
              + compute_A(i - 1, j / 2)
              + compute_A(i - 1, j / 3);

    cache[idx]  = val;
    computed[idx] = 1;
    return val;
}

/* External function requested: a(idx_t n)                                */
/* n is the index in the antidiagonal reading (starting at 0).           */
seq_t a(idx_t n)
{
    /* Find which diagonal contains position n. */
    size_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= (size_t)n)
        ++d;

    size_t prev_total = d * (d + 1) / 2;      /* elements before this diagonal */
    size_t offset     = (size_t)n - prev_total;   /* position inside the diagonal */

    size_t i = offset;              /* row index */
    size_t j = d - offset;          /* column index */

    return compute_A(i, j);
}

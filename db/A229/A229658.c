/*
 * A229658  8-section a(8n+k) gives k-th differences of a for k=0..7 with
 *          a(n)=0 for n<7 and a(7)=1.
 * https://oeis.org/A229658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, eigen, look
 * Formula:   not stated
 * Range:     i8 n <= 86, i16 n <= 1182, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A229658
   Formula: a(8*n + k) = Σ_{j=0..k} (-1)^j · C(k,j) · a(n + k - j)
   for k = 0..7, a(n) = 0 for n < 7, a(7) = 1.
   The function computes any a(n) (n ≥ 0), memoising results in blocks
   of 8 to avoid recomputation.  It works for 64‑bit or arbitrary‑precision
   seq_t types. */

#include <stdlib.h>
#include <stddef.h>

/* Binomial coefficients C(k,j) for k, j ≤ 7 */
static const unsigned char comb[8][8] = {
    {1,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,2,1,0,0,0,0,0},
    {1,3,3,1,0,0,0,0},
    {1,4,6,4,1,0,0,0},
    {1,5,10,10,5,1,0,0},
    {1,6,15,20,15,6,1,0},
    {1,7,21,35,35,21,7,1}
};

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0; /* number of terms stored */

/* Initialise cache with the first 8 values */
static void init_cache(void) {
    cache_cap = 8;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    for (idx_t i = 0; i < 7; ++i)
        cache[i] = 0;
    cache[7] = 1;
}

/* Ensure that the cache contains values at least up to index `n` (inclusive). */
static void ensure_cache(idx_t n) {
    if (!cache) {
        init_cache();          /* first use */
    }

    /* Compute up to the next full block that contains index n. */
    idx_t target = ((n) / 8 + 1) * 8;        /* guarantees target > n */
    if ((idx_t)target > cache_cap) {
        /* Remember how many full blocks we already have */
        size_t old_blocks  = cache_cap / 8;
        size_t new_blocks  = target / 8;

        cache = (seq_t *)realloc(cache, target * sizeof(seq_t));

        /* Compute new blocks, starting after the last processed block */
        for (size_t block = old_blocks; block < new_blocks; ++block) {
            idx_t m = (idx_t)block;           /* block number */
            for (idx_t k = 0; k < 8; ++k) {  /* offset within the block */
                seq_t sum = 0;
                for (idx_t j = 0; j <= k; ++j) {
                    idx_t idx = m + k - j;    /* referenced index */
                    seq_t term = cache[idx];
                    if (j & 1)                  /* (-1)^j */
                        term = -term;
                    term *= (seq_t)comb[k][j];
                    sum += term;
                }
                cache[8 * m + k] = sum;
            }
        }
        cache_cap = target;
    }
}

/* Public function: returns a(n) */
seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}

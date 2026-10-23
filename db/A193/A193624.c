/*
 * A193624  Number of ways n triples can sit in a row without any siblings
 *          next to each other.
 * https://oeis.org/A193624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(9*n^3-9*n^2+8*n+8)*a(n-1)
 *            -108*(n-1)*(n^2-11*n+16)*a(n-2) -3024*(n-1)*(n-2)^2*a(n-3)
 *            +5184*(n-1)*(n-2)*(n-3)*a(n-4) Initial values: a(0)=1, a(1)=0,
 *            a(2)=72, a(3)=37584
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External types supplied by the build. */
extern seq_t a(idx_t n);

/* Static cache for memoisation */
static seq_t *cache = NULL;         /* dynamically allocated array */
static idx_t cache_cap = 0;         /* allocated size in elements   */
static idx_t cached_upto = 0;       /* first invalid index [0..cached_upto-1] are computed */

/* Expand the cache to hold at least up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;            /* already large enough */

    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_mem) abort(); /* unrecoverable */
    cache = new_mem;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Compute missing terms up to n. */
    for (idx_t i = cached_upto; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)0;
        } else if (i == 2) {
            cache[2] = (seq_t)72;
        } else if (i == 3) {
            cache[3] = (seq_t)37584;
        } else {
            seq_t z = (seq_t)i;

            seq_t n1 = z - (seq_t)1;
            seq_t n2 = z * z;
            seq_t n3 = n2 * z;
            seq_t n_2 = z - (seq_t)2;
            seq_t n_3 = z - (seq_t)3;

            /* coeff1 = 3*(9*n^3-9*n^2+8*n+8) */
            seq_t tmp1 = (seq_t)9 * n3 - (seq_t)9 * n2 + (seq_t)8 * z + (seq_t)8;
            seq_t coeff1 = (seq_t)3 * tmp1;

            /* coeff2 = 108*(n-1)*(n^2-11*n+16) */
            seq_t inside2 = n2 - (seq_t)11 * z + (seq_t)16;
            seq_t coeff2 = (seq_t)108 * n1 * inside2;

            /* coeff3 = 3024*(n-1)*(n-2)^2 */
            seq_t inside3 = n_2 * n_2;
            seq_t coeff3 = (seq_t)3024 * n1 * inside3;

            /* coeff4 = 5184*(n-1)*(n-2)*(n-3) */
            seq_t coeff4 = (seq_t)5184 * n1 * n_2 * n_3;

            seq_t term1 = coeff1 * cache[i-1];
            seq_t term2 = coeff2 * cache[i-2];
            seq_t term3 = coeff3 * cache[i-3];
            seq_t term4 = coeff4 * cache[i-4];

            seq_t val = term1;
            val -= term2;
            val -= term3;
            val += term4;

            cache[i] = val;
        }
    }
    cached_upto = n + 1;
    return cache[n];
}

/*
 * A151760  G.f.: Theta^4, where Theta = Sum_{k>=0} x^(2^k).
 * https://oeis.org/A151760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of ordered quadruples (i,j,k,l) of powers of 2
 * such that 2^i + 2^j + 2^k + 2^l = n.
 * The function enumerates all powers of 2 up to n and counts all
 * ordered combinations; the counts are cached so repeated calls are
 * O(1) once the cache has been built for the largest n requested. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* cached counts */
static idx_t cache_len = 0;      /* number of valid entries in cache */

/* build or rebuild the cache up to the given maximum n */
static void build_cache(idx_t max_n)
{
    /* allocate memory for max_n + 1 entries */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(max_n + 1) * sizeof(seq_t));
    if (!tmp) {                     /* allocation failed – abort */
        return;
    }
    /* zero out all entries (old cache may contain old data) */
    for (idx_t i = 0; i <= max_n; ++i) {
        tmp[i] = 0;
    }

    /* generate list of powers of two <= max_n */
    idx_t pw = 1;
    idx_t powers[64];
    idx_t pcnt = 0;
    while (pw <= max_n && pcnt < (idx_t)(sizeof(powers)/sizeof(powers[0]))) {
        powers[pcnt++] = pw;
        pw <<= 1;
    }

    /* enumerate all quadruples (ordered) of powers of two */
    for (idx_t i = 0; i < pcnt; ++i) {
        for (idx_t j = 0; j < pcnt; ++j) {
            for (idx_t k = 0; k < pcnt; ++k) {
                for (idx_t l = 0; l < pcnt; ++l) {
                    /* use 128‑bit intermediate to avoid overflow */
                    __int128 s = (__int128)powers[i] + powers[j] +
                                 powers[k] + powers[l];
                    if ((idx_t)s <= max_n) {
                        tmp[(idx_t)s] += 1;
                    }
                }
            }
        }
    }

    cache     = tmp;
    cache_len = max_n + 1;
}

seq_t a(idx_t n)
{
    if (cache_len <= n) {
        build_cache(n);
    }
    return cache[n];
}

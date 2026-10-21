/*
 * A202824  Expansion of e.g.f.: exp( (1+x)^4 - 1 ).
 * https://oeis.org/A202824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=28, a(3)=232. For n≥4, a(n) = 4*a(n‑1) +
 *            12*(n‑1)*a(n‑2) + 12*(n‑2)*(n‑1)*a(n‑3) +
 *            4*(n‑3)*(n‑2)*(n‑1)*a(n‑4)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/grow_seq.h"

static seq_t *a_cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* initialise cache with first four terms if needed */
    if (cache_size == 0) {
        grow_seq(&a_cache, &cache_cap, 4);
        a_cache[0] = 1;
        a_cache[1] = 4;
        a_cache[2] = 28;
        a_cache[3] = 232;
        cache_size = 4;
    }

    /* return if already computed */
    if (n < cache_size)
        return a_cache[n];

    /* expand cache up to n */
    idx_t needed = n + 1;
    if (needed > cache_cap)
        grow_seq(&a_cache, &cache_cap, needed);

    /* compute missing values */
    for (idx_t i = cache_size; i < needed; ++i) {
        /* i ≥ 4 because cache_size == 4 for i=4 first iteration */
        seq_t term1 = 4 * a_cache[i - 1];
        seq_t term2 = 12 * ((seq_t)(i - 1)) * a_cache[i - 2];
        seq_t term3 = 12 * ((seq_t)(i - 2)) * ((seq_t)(i - 1)) * a_cache[i - 3];
        seq_t term4 = 4 * ((seq_t)(i - 3)) * ((seq_t)(i - 2)) * ((seq_t)(i - 1)) * a_cache[i - 4];
        a_cache[i] = term1 + term2 + term3 + term4;
    }

    cache_size = needed;
    return a_cache[n];
}

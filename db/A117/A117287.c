/*
 * A117287  Numbers k for which the cototient k-phi(k) is a heptagonal number.
 * https://oeis.org/A117287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 60, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, grow_seq, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117287: numbers k such that k-phi(k) is a heptagonal number.
   There is no closed form; the function builds terms lazily and caches
   them. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/phi.h"
#include "../utils/floor_sqrt.h"
#include "../utils/grow_seq.h"

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_k = 0;

/* test whether v is a heptagonal number: v = t(5t-3)/2 for some t >= 0 */
static bool is_heptagonal(seq_t v)
{
    if (v == 0) return true;               /* H0 = 0 */
    seq_t D = (seq_t)9 + (seq_t)40 * v;    /* discriminant 9 + 40v */
    seq_t sqrtD = floor_sqrt(D);
    if (sqrtD * sqrtD != D) return false;  /* not a perfect square */
    return (((seq_t)3 + sqrtD) % (seq_t)10) == 0;
}

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* behaviour below offset is irrelevant */

    while (cache_len < (size_t)n) {
        seq_t k = (seq_t)(++last_k);
        seq_t phi_k = phi((idx_t)k);
        seq_t cotot = k - phi_k;
        if (is_heptagonal(cotot)) {
            if (cache_len >= cache_cap) {
                grow_seq(&cache, &cache_cap, cache_len + 1);
            }
            cache[cache_len++] = k;
        }
    }
    return cache[(size_t)n - 1];
}

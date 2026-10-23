/*
 * A021555  Decimal expansion of 1/551.
 * https://oeis.org/A021555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 551) mod 10 (nth decimal digit of 1/551)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *digit_cache = NULL;   /* cache of computed digits a(0)…a(k-1) */
static size_t cache_len  = 0;       /* number of digits currently stored   */
static size_t cache_cap  = 0;       /* allocated capacity                   */
static uint64_t curr_rem = 1;       /* remainder after computing cache_len-1 */

/* sequence element: nth decimal digit after the point in 1/551 */
seq_t a(idx_t n)
{
    /* ensure cache is allocated */
    if (!digit_cache) {
        cache_cap = 16;
        digit_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* no need to initialise, values will be filled below   */
    }

    /* grow cache if necessary */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < need) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(digit_cache, new_cap * sizeof(seq_t));
        digit_cache = tmp;
        cache_cap = new_cap;
    }

    /* fill cache up to index n (inclusive) */
    if (need > cache_len) {
        for (size_t i = cache_len; i < need; ++i) {
            uint64_t r = curr_rem * 10ULL;          /* 10 * previous remainder */
            seq_t digit = (seq_t)(r / 551ULL);       /* quotient digit */
            curr_rem = r % 551ULL;                  /* new remainder for next step */
            digit_cache[i] = digit;
        }
        cache_len = need;
    }

    return digit_cache[(size_t)n];
}

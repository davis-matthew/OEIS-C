/*
 * A042832  Numerators of continued fraction convergents to sqrt(947).
 * https://oeis.org/A042832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of continued‑fraction convergents to sqrt(947)
   It uses the recurrence:
   p_{-2}=0, p_{-1}=1,
   p_k = a_k * p_{k-1} + p_{k-2}  for k ≥ 0,
   with a_0 = 30 and the periodic part
   a_1,a_2,…,a_22 = [1,3,2,2,2,1,4,1,7,1,29,1,7,1,4,1,2,2,2,3,1,60].
   The periodic part has length 22 and repeats for all k ≥ 1.
   The function memoizes results so subsequent calls are O(1). */

#include <stdlib.h>

static const unsigned short period[22] = {
    1,3,2,2,2,1,4,1,7,1,29,1,7,1,4,1,2,2,2,3,1,60
};

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Expand cache until it contains index n */
    while (cache_len <= (size_t)n)
    {
        /* Ensure space for at least one more element */
        if (cache_cap < cache_len + 1)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            if (new_cap < cache_len + 1) new_cap = cache_len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;            /* allocation failure – unlikely */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Determine indices for recurrence */
        seq_t coeff;
        if (cache_len == 0)               /* k = 0 */
            coeff = 30;
        else                               /* k ≥ 1 */
            coeff = period[(cache_len - 1) % 22];

        seq_t prev, prev2;
        if (cache_len == 0) {             /* initial step: p_{-2}=0, p_{-1}=1 */
            prev2 = 0;
            prev  = 1;
        }
        else if (cache_len == 1) {        /* next step: p_{-2}=1, p_{-1}=p0 */
            prev2 = 1;
            prev  = cache[0];
        }
        else {                            /* general case */
            prev2 = cache[cache_len - 2];
            prev  = cache[cache_len - 1];
        }

        /* recurrence: p_k = coeff * prev + prev2 */
        seq_t val = coeff * prev + prev2;
        cache[cache_len++] = val;
    }

    return cache[(size_t)n];
}

/*
 * A081645  Integers congruent to 0, 1, 4, 9, 16 or 25 (mod 36) which are not
 *          squares.
 * https://oeis.org/A081645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n‑th integer such that n mod 36 is 0,1,4,9,16 or 25
 *            * and n is not a perfect square
 * Range:     u8 n <= 32, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_isqrt_seq.h"   /* provide _isqrt_seq(seq_t) */

static seq_t* cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static seq_t last_checked = 0;
static bool initialized = false;

static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        cache_len = 0;
        cache_cap = 0;
        cache = NULL;
        last_checked = 0;
        initialized = true;
    }
    if (n == 0) return 0;              /* offset is 1; behaviour for n=0 is irrelevant */

    if (n <= cache_len) return cache[n-1];

    grow_cache(n);

    seq_t cand = last_checked + 1;
    while (cache_len < n) {
        seq_t r = cand % (seq_t)36;
        if (r == 0 || r == 1 || r == 4 || r == 9 || r == 16 || r == 25) {
            seq_t s = _isqrt_seq(cand);
            if (s * s != cand) {
                cache[cache_len++] = cand;
            }
        }
        cand++;
    }
    last_checked = cand - 1;
    return cache[n-1];
}

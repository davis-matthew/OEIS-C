/*
 * A117821  a(0) = a(1) = 1; for n >= 2, a(n) = a(n-1) + a(n-2) - n if that
 *          number is positive and not already in the sequence, otherwise a(n)
 *          = a(n-1) + a(n-2) + n.
 * https://oeis.org/A117821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=1, a(1)=1; for n≥2, a(n)=a(n‑1)+a(n‑2)-n if that number>0 and not already
   in the sequence, else a(n)=a(n‑1)+a(n‑2)+n */

#include <stdlib.h>
#include <stdbool.h>

/* dynamic cache for memoising the sequence */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity */

/* check whether val already occurs among cache[0..up_to-1] */
static bool contains(seq_t val, size_t up_to)
{
    for (size_t i = 0; i < up_to; ++i)
        if (cache[i] == val)
            return true;
    return false;
}

/* compute and store terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        if (cache_cap < 2) cache_cap = 2;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) exit(1);
        cache[0] = 1;
        cache[1] = 1;
        cache_len = 2;
    }

    if (cache_len > (size_t)n) return;   /* already computed */

    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        seq_t sub = (seq_t)i;
        seq_t cand;

        if (sum > sub) {
            seq_t tryminus = sum - sub;
            if (tryminus > (seq_t)0 && !contains(tryminus, i))
                cand = tryminus;
            else
                cand = sum + sub;
        } else {
            cand = sum + sub;
        }

        /* grow capacity if needed */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
            if (!tmp) exit(1);
            cache = tmp;
        }
        cache[cache_len++] = cand;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 1;
    ensure_cache(n);
    return cache[(size_t)n];
}

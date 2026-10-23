/*
 * A164494  Number of binary strings of length n with no substrings equal to
 *          0010 0101 or 1010.
 * https://oeis.org/A164494
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 78, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Automaton recurrence for OEIS sequence A164494
 * Recurrence: a(n) = number of binary strings of length n
 *            with no substring 0010, 0101 or 1010.
 * Offset: 4                     (a(4) = 13)
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache      = NULL;
    static idx_t  cache_size = 0;   /* number of stored entries (indices 0 … cache_size-1) */
    static idx_t  cur_len    = 0;   /* current length of strings represented by dp_cur */
    static seq_t dp_cur[8] = {0};   /* state counts for strings of length cur_len */
    static seq_t dp_next[8] = {0};  /* buffer for next length */

    /* ----------- initialise everything on the first call ----------- */
    if (cache == NULL) {
        /* store indices 0 … 4 inclusive */
        cache_size = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;   /* OOM fallback */

        /* a(0) = 1 (empty string) */
        cache[0] = (seq_t)1;

        /* initial DP: only the empty string */
        memset(dp_cur, 0, sizeof(dp_cur));
        dp_cur[0] = (seq_t)1;
        cur_len = 0;

        /* precompute terms up to index 4 */
        for (idx_t l = 0; l < 4; ++l) {
            memset(dp_next, 0, sizeof(dp_next));
            for (int s = 0; s < 8; ++s) {
                seq_t val = dp_cur[s];
                if (!val) continue;
                for (int b = 0; b < 2; ++b) {
                    int new_s = (s << 1) | b;
                    new_s <<= 0;          /* keep 3 bits: new_s & 7 */
                    new_s &= 7;
                    /* check forbidden 4‑bit pattern after appending b */
                    if (cur_len >= 3) {
                        int pat = ((s << 1) | b) & 0xF;  /* last 4 bits */
                        if (pat == 0b0010 || pat == 0b0101 || pat == 0b1010)
                            continue;    /* forbidden substring */
                    }
                    dp_next[new_s] += val;
                }
            }
            memcpy(dp_cur, dp_next, sizeof(dp_cur));
            ++cur_len;                           /* now strings of length l+1 */
            seq_t total = 0;
            for (int s = 0; s < 8; ++s) total += dp_cur[s];
            cache[cur_len] = total;   /* store a(cur_len) */
        }
    }

    /* ----------- return cached value if available ----------- */
    if (n < cache_size)
        return cache[n];

    /* ----------- extend cache and DP up to requested n ----------- */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!new_cache) return (seq_t)0;   /* OOM fallback */
    cache = new_cache;
    cache_size = n + 1;

    for (idx_t l = cur_len; l < n; ++l) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int s = 0; s < 8; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            for (int b = 0; b < 2; ++b) {
                int new_s = (s << 1) | b;
                new_s &= 7;
                if (cur_len >= 3) {
                    int pat = ((s << 1) | b) & 0xF;
                    if (pat == 0b0010 || pat == 0b0101 || pat == 0b1010)
                        continue;
                }
                dp_next[new_s] += val;
            }
        }
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
        ++cur_len;
        seq_t total = 0;
        for (int s = 0; s < 8; ++s) total += dp_cur[s];
        cache[cur_len] = total;
    }

    return cache[n];
}

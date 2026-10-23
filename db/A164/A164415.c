/*
 * A164415  Number of binary strings of length n with no substrings equal to
 *          0000, 0001, or 1100.
 * https://oeis.org/A164415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n with no substrings
 *            0000, 0001, or 1100. Computation uses dynamic programming over
 *            the last three bits; forbids are checked only when the string
 *            length reaches 4
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 80, u128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values and current DP state */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static seq_t dp_current[8];
    static idx_t dp_len = 0;  /* length whose dp_current corresponds to */

    /* initialise on first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_size = 1;
        for (int i = 0; i < 8; ++i) dp_current[i] = (seq_t)0;
        dp_current[0] = (seq_t)1;
        dp_len = 0;
    }

    if (n < cache_size) return cache[n];

    /* grow cache if needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1)) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* forbidden 4‑bit patterns: 0000, 0001, 1100 */
    static const unsigned char forb[16] = {1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0};

    seq_t dp_next[8];

    for (idx_t len = dp_len + 1; len <= n; ++len) {
        for (int i = 0; i < 8; ++i) dp_next[i] = (seq_t)0;

        for (int s = 0; s < 8; ++s) {
            seq_t val = dp_current[s];
            if (val == (seq_t)0) continue;

            /* append bit 0 */
            int pat0 = (s << 1);
            if (!(len >= 4 && forb[pat0])) {
                int ns0 = pat0 & 7;
                dp_next[ns0] += val;
            }

            /* append bit 1 */
            int pat1 = (s << 1) | 1;
            if (!(len >= 4 && forb[pat1])) {
                int ns1 = pat1 & 7;
                dp_next[ns1] += val;
            }
        }

        /* sum all DP entries for this length */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < 8; ++i) sum += dp_next[i];
        cache[len] = sum;

        /* advance DP state */
        for (int i = 0; i < 8; ++i) dp_current[i] = dp_next[i];
    }

    dp_len = n;
    cache_size = (size_t)(n + 1);

    return cache[n];
}

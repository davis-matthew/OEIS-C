/*
 * A077956  Expansion of 1/(1-x+2*x^2+2*x^3).
 * https://oeis.org/A077956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=-1, a(n)=a(n-1)-2*a(n-2)-2*a(n-3)
 * Range:     i8 n <= 8, i16 n <= 19, i32 n <= 39, i64 n <= 78, i128 none,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

#ifdef __cplusplus
static std::vector<seq_t> seq_cache;
#else
static seq_t *seq_cache = NULL;
static size_t seq_cache_len = 0;
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    if (seq_cache.size() <= (size_t)n) {
        if (seq_cache.empty()) {
            seq_cache.push_back((seq_t)1);   /* a(0) */
            seq_cache.push_back((seq_t)1);   /* a(1) */
            seq_cache.push_back((seq_t)-1);  /* a(2) */
        }
        for (size_t i = seq_cache.size(); i <= (size_t)n; ++i) {
            seq_t val = seq_cache[i-1] - (seq_t)2 * seq_cache[i-2] - (seq_t)2 * seq_cache[i-3];
            seq_cache.push_back(val);
        }
    }
    return seq_cache[(size_t)n];
#else
    if (n >= seq_cache_len) {
        size_t newlen = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, newlen * sizeof(seq_t));
        if (!tmp) abort();               /* out‑of‑memory */
        seq_cache = tmp;
        size_t i = seq_cache_len;

        /* fill missing base cases */
        if (i == 0) { seq_cache[0] = (seq_t)1; i = 1; }
        if (i == 1) { seq_cache[1] = (seq_t)1; i = 2; }
        if (i == 2) { seq_cache[2] = (seq_t)-1; i = 3; }

        /* compute remaining terms */
        for (size_t j = i; j <= (size_t)n; ++j) {
            seq_cache[j] = seq_cache[j-1] - (seq_t)2 * seq_cache[j-2] - (seq_t)2 * seq_cache[j-3];
        }
        seq_cache_len = newlen;
    }
    return seq_cache[(size_t)n];
#endif
}

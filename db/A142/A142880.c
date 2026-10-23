/*
 * A142880  a(n) = 7*a(n-3) - a(n-6).
 * https://oeis.org/A142880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=2, a(3)=3, a(4)=8, a(5)=13,
   a(n)=7*a(n-3)-a(n-6)  for n≥6.
   This function memoizes the values so that subsequent calls are O(1). */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

/* C++ version uses std::vector; C version uses malloc/realloc. */
#ifdef __cplusplus
static std::vector<seq_t> a_cache;
#else
static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;
static size_t a_cache_cap = 0;

static void a_ensure_capacity(size_t needed)
{
    if (needed < a_cache_cap) return;
    size_t new_cap = a_cache_cap ? (a_cache_cap << 1) : 8;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) exit(1);
    a_cache = tmp;
    a_cache_cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

#ifdef __cplusplus
    if (a_cache.size() > idx) return a_cache[idx];
    size_t old = a_cache.size();
    a_cache.resize(idx + 1);
    static const seq_t init_vals[6] = {
        (seq_t)0, (seq_t)1, (seq_t)2,
        (seq_t)3, (seq_t)8, (seq_t)13
    };
    for (size_t i = old; i <= idx; ++i) {
        if (i < 6) a_cache[i] = init_vals[i];
        else a_cache[i] = (seq_t)7 * a_cache[i-3] - a_cache[i-6];
    }
    return a_cache[idx];
#else
    if (a_cache_len > idx) return a_cache[idx];
    a_ensure_capacity(idx);
    static const seq_t init_vals[6] = {
        (seq_t)0, (seq_t)1, (seq_t)2,
        (seq_t)3, (seq_t)8, (seq_t)13
    };
    for (size_t i = a_cache_len; i <= idx; ++i) {
        if (i < 6) a_cache[i] = init_vals[i];
        else a_cache[i] = (seq_t)7 * a_cache[i-3] - a_cache[i-6];
    }
    a_cache_len = idx + 1;
    return a_cache[idx];
#endif
}

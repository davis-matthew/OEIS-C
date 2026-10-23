/*
 * A122092  a(n) = (n-2)*a(n-1) - (n-1)*a(n-2), with a(0)=1, a(1)=1.
 * https://oeis.org/A122092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-2)*a(n-1) - (n-1)*a(n-2), a(0)=1, a(1)=1
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 15, i64 n <= 23, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

#ifdef __cplusplus
/* C++ compilation: use std::vector to store arbitrary‑precision integers */
static std::vector<seq_t> cache;
static void ensure_capacity(idx_t n)
{
    if (cache.empty()) {
        cache.reserve(2);
        cache.push_back(1);
        cache.push_back(1);
    }
    while (cache.size() <= n) {
        idx_t k = cache.size();           /* next index to compute */
        seq_t v1 = (seq_t)(k - 2) * cache[k - 1];
        seq_t v2 = (seq_t)(k - 1) * cache[k - 2];
        cache.push_back(v1 - v2);
    }
}
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 1;
    ensure_capacity(n);
    return cache[n];
}
#else
/* C compilation: dynamic array of seq_t, works for both built‑in and
   arbitrary‑precision integer types that support the usual arithmetic
   operators. */
static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;   /* allocated capacity */
static size_t seq_len = 0;   /* number of terms currently stored */

static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    if (new_cap < (size_t)n + 1)
        new_cap = (size_t)n + 1;
    seq_t *tmp = realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap != 0)
        abort();                /* allocation failure */
    seq_cache = tmp;
    seq_cap   = new_cap;
    if (seq_len == 0) {        /* first use: initialize a(0)=a(1)=1 */
        seq_cache[0] = 1;
        seq_cache[1] = 1;
        seq_len = 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 1;
    ensure_capacity(n);
    if ((size_t)n < seq_len)
        return seq_cache[n];
    for (idx_t k = seq_len; k <= n; ++k) {
        seq_t v1 = (seq_t)(k - 2) * seq_cache[k - 1];
        seq_t v2 = (seq_t)(k - 1) * seq_cache[k - 2];
        seq_cache[k] = v1 - v2;
    }
    seq_len = (size_t)n + 1;
    return seq_cache[n];
}
#endif

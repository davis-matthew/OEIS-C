/*
 * A173735  a(n) = (10^n + 26)/9.
 * https://oeis.org/A173735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3; a(n)=10*a(n-1)-26.  (Equivalent to a(n)=(10^n+26)/9.) */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

#if !defined(__cplusplus)
static seq_t *cache_c = NULL;
static idx_t cache_size_c = 0;
static idx_t cache_capacity_c = 0;
#else
static std::vector<seq_t> cache_cpp;
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    if (cache_cpp.empty())
        cache_cpp.push_back(3);
    while (cache_cpp.size() <= n) {
        seq_t val = cache_cpp.back() * 10 - 26;
        cache_cpp.push_back(val);
    }
    return cache_cpp[n];
#else
    if (cache_c == NULL) {
        cache_capacity_c = 1;
        cache_c = (seq_t*)malloc(sizeof(seq_t) * cache_capacity_c);
        cache_c[0] = 3;
        cache_size_c = 1;
    }
    while (cache_size_c <= n) {
        idx_t idx = cache_size_c;           /* current count of stored elements */
        seq_t val = cache_c[idx-1] * 10 - 26;
        if (cache_size_c == cache_capacity_c) {
            cache_capacity_c <<= 1;
            cache_c = (seq_t*)realloc(cache_c, sizeof(seq_t) * cache_capacity_c);
        }
        cache_c[cache_size_c++] = val;
    }
    return cache_c[n];
#endif
}

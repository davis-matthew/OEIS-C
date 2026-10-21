/*
 * A052585  E.g.f. 1/(1-x-2*x^2).
 * https://oeis.org/A052585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052585: Generalized factorial numbers with egf 1/(1-x-2x^2).
   Formula: a(0)=1, a(1)=1,
   a(n+2) = (2*n^2 + 6*n + 4)*a(n) + (n+2)*a(n+1). */

#ifdef __cplusplus
#include <vector>
seq_t a(idx_t n)
{
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(2);
        cache.push_back((seq_t)1); /* a(0) */
        cache.push_back((seq_t)1); /* a(1) */
    }
    while ((idx_t)cache.size() <= n) {
        idx_t m = cache.size() - 2;                /* index for a(m) and a(m+1) */
        seq_t ms   = (seq_t)m;
        seq_t f1   = (seq_t)2*ms*ms + (seq_t)6*ms + (seq_t)4; /* 2*m^2+6*m+4 */
        seq_t f2   = (seq_t)2 + ms;                /* 2+m */
        seq_t term = f1 * cache[m] + f2 * cache[m+1];
        cache.push_back(term);
    }
    return cache[n];
}
#else
#include <stddef.h>
#include <stdlib.h>

static seq_t cache[20001];
static idx_t cache_size = 2;
static int cache_initialised = 0;

seq_t a(idx_t n)
{
    if (!cache_initialised) {
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)1;   /* a(1) */
        cache_initialised = 1;
    }
    while (cache_size <= n) {
        idx_t m = cache_size - 2;                 /* index for a(m) and a(m+1) */
        seq_t ms   = (seq_t)m;
        seq_t f1   = (seq_t)2*ms*ms + (seq_t)6*ms + (seq_t)4; /* 2*m^2+6*m+4 */
        seq_t f2   = (seq_t)2 + ms;                /* 2+m */
        cache[cache_size] = f1 * cache[m] + f2 * cache[m+1];
        ++cache_size;
    }
    return cache[n];
}
#endif

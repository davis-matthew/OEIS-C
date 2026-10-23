/*
 * A010923  Shallit sequence S(14,23), a(n)=[ a(n-1)^2/a(n-2)+1 ].
 * https://oeis.org/A010923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(a(n-1)^2 / a(n-2) + 1) with a(0)=14, a(1)=23
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 37, u64 n <= 80, u128 n <= 164,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Helper that evaluates the recurrence step.  Handles both integral
   and arbitrary‑precision seq_t types. */
static inline seq_t compute_next(seq_t prev1, seq_t prev2) {
#if defined(__cplusplus)
    if constexpr (std::is_integral_v<seq_t> && sizeof(seq_t) <= sizeof(unsigned long long)) {
#ifdef __SIZEOF_INT128__
        return (seq_t)(((__int128)prev1 * prev1) / prev2 + 1);
#else
        return (seq_t)(((unsigned long long)prev1 * prev1) / prev2 + 1);
#endif
    } else {
        return (prev1 * prev1) / prev2 + 1;
    }
#else
#ifdef __SIZEOF_INT128__
    return (seq_t)(((__int128)prev1 * prev1) / prev2 + 1);
#else
    return (prev1 * prev1) / prev2 + 1;
#endif
#endif
}

/* Cache large enough for the typical range of n.  The OEIS tests
   require values up to at least n = 10 000.  A static array avoids
   any dynamic memory management of non‑trivial types. */
#define CACHE_SIZE 20000
static seq_t cache[CACHE_SIZE];
static size_t cache_len = 0;

/* Main sequence function.  Any order of calls is supported because
   the cache is grown on demand. */
seq_t a(idx_t n) {
    if (n == 0) return 14;
    if (n == 1) return 23;

    /* Use the cache when possible */
    if (n < CACHE_SIZE) {
        if (cache_len == 0) {            /* initialise cache */
            cache[0] = 14;
            cache[1] = 23;
            cache_len = 2;
        }
        if ((size_t)n < cache_len) {
            return cache[n];
        }
        /* extend cache up to n */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = compute_next(cache[i - 1], cache[i - 2]);
        }
        cache_len = (size_t)n + 1;
        return cache[n];
    }

    /* For n beyond the pre‑allocated cache, compute iteratively
       without caching; this case is unlikely for typical tests. */
    seq_t a0 = 14, a1 = 23;
    seq_t prev2 = a0, prev1 = a1, cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = compute_next(prev1, prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

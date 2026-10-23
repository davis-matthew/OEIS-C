/*
 * A177259  Number of derangements of {1,2,...,n} having no adjacent 3-cycles
 *          (an adjacent 3-cycle is a cycle of the form (i,i+1,i+2)).
 * https://oeis.org/A177259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*a(n-1) + (n-1)*a(n-2) + a(n-3) + (n-1)*a(n-4) +
 *            2*a(n-6) initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=9,
 *            a(5)=41, a(6)=258
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <vector>
static std::vector<seq_t> cache;
static std::vector<char> computed;
static void ensure_size(size_t n) {
    if (cache.size() <= n) {
        cache.resize(n + 1);
        computed.resize(n + 1, 0);
    }
}
#else
static seq_t *cache = NULL;
static unsigned char *computed = NULL;
static size_t cache_size = 0;
static void ensure_size(size_t n) {
    if (cache_size <= n) {
        size_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        computed = (unsigned char *)realloc(computed, new_size * sizeof(unsigned char));
        for (size_t i = cache_size; i < new_size; ++i) computed[i] = 0;
        cache_size = new_size;
    }
}
#endif

seq_t a(idx_t n) {
    if (n < 7) {
        /* Return pre‑computed values for 0 ≤ n < 7 */
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)0;
            case 2: return (seq_t)1;
            case 3: return (seq_t)1;
            case 4: return (seq_t)9;
            case 5: return (seq_t)41;
            case 6: return (seq_t)258;
        }
        return (seq_t)0; /* unreachable */
    }

    size_t idx = (size_t)n;
    ensure_size(idx);

    for (size_t i = 0; i <= idx; ++i) {
        if (computed[i]) continue;          /* already computed */
        if (i < 7) {                        /* base cases */
            seq_t v;
            switch (i) {
                case 0: v = (seq_t)1; break;
                case 1: v = (seq_t)0; break;
                case 2: v = (seq_t)1; break;
                case 3: v = (seq_t)1; break;
                case 4: v = (seq_t)9; break;
                case 5: v = (seq_t)41; break;
                case 6: v = (seq_t)258; break;
                default: v = (seq_t)0; break; /* should not happen */
            }
            cache[i] = v;
            computed[i] = 1;
        } else {
            /* recurrence holds for i ≥ 7 */
            seq_t term0 = (seq_t)(i - 1) * cache[i - 1];
            seq_t term1 = (seq_t)(i - 1) * cache[i - 2];
            seq_t term2 = cache[i - 3];
            seq_t term3 = (seq_t)(i - 1) * cache[i - 4];
            seq_t term4 = (seq_t)2 * cache[i - 6];
            cache[i] = term0 + term1 + term2 + term3 + term4;
            computed[i] = 1;
        }
    }

    return cache[idx];
}

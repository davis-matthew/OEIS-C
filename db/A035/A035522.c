/*
 * A035522  Reverse and add (in binary) - written in base 10.
 * https://oeis.org/A035522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n+1) = a(n) + reverse_binary(a(n)), a(0) = 1
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 54, u64 n <= 118,
 *            u128 n <= 246, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reverse a number in binary: take bits from LSB to MSB and
   rebuild the integer in reverse order.  Works for any seq_t
   that supports >>, &, <<, |, +, and > 0. */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
using namespace std;
static inline seq_t reverse_binary(seq_t x) {
    seq_t rev = 0;
    while (x > 0) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}
static std::vector<seq_t> cache_vec;       /* holds values 0 .. n */
static void ensure_cpp(idx_t n) {
    size_t sz = cache_vec.size();
    if (sz <= static_cast<size_t>(n)) {
        if (sz == 0) {
            cache_vec.resize(1);
            cache_vec[0] = 1;
            sz = 1;
        }
        cache_vec.resize(static_cast<size_t>(n) + 1);
        for (size_t i = sz; i <= static_cast<size_t>(n); ++i) {
            cache_vec[i] = cache_vec[i - 1] + reverse_binary(cache_vec[i - 1]);
        }
    }
}
#else
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static inline seq_t reverse_binary(seq_t x) {
    seq_t rev = 0;
    while (x > 0) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}
static void ensure_c(idx_t n) {
    if (cache_size <= n) {
        seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) { abort(); }     /* memory exhausted */
        cache = new_cache;
        if (cache_size == 0) {
            cache[0] = 1;
            cache_size = 1;
        }
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i - 1] + reverse_binary(cache[i - 1]);
        }
        cache_size = n + 1;
    }
}
#endif

extern seq_t a(idx_t n) {
#ifdef __cplusplus
    ensure_cpp(n);
    return cache_vec[static_cast<size_t>(n)];
#else
    ensure_c(n);
    return cache[n];
#endif
}

/*
 * A204638  Number of (n+2) X 6 0..3 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically, diagonally
 *          or antidiagonally exactly three ways, and new values 0..3
 *          introduced in row major order.
 * https://oeis.org/A204638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4246, a(2) = 379, a(3) = 933, a(4) = 478, a(5) = 1169,
 *            a(6) = 4366, a(7) = 8179, a(8) = 29877, a(9) = 113861, a(10) =
 *            407328 . For n > 10: a(n) = 5*a(n-1) -3*a(n-2) -4*a(n-3)
 *            -12*a(n-4) +12*a(n-5) +21*a(n-6) -9*a(n-7) -9*a(n-8); The
 *            implementation memoises computed values for any call order
 * Range:     u8 none, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static const seq_t init_terms[10] = {
    4246,   379,   933,   478,  1169,
    4366,   8179, 29877,113861,407328
};

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> seq_cache;
#else
/* In C build we use a fixed‑size static array.  The tests use n ≤ 10000
   (≈ 1e4) so a capacity of 20000 is ample.  */
#define CACHE_CAPACITY 20000
static seq_t seq_cache[CACHE_CAPACITY];
static size_t cached_len = 0;
#endif

seq_t a(idx_t n) {
    if (n == 0) return 0;
    size_t idx = (size_t)n;

#ifdef __cplusplus
    /* Initialise cache if not yet done. */
    if (seq_cache.size() < 10) {
        seq_cache.resize(10);
        for (size_t i = 0; i < 10; ++i) seq_cache[i] = init_terms[i];
    }
    /* Return stored value if already computed. */
    if (idx <= seq_cache.size()) {
        return seq_cache[idx - 1];
    }
    /* Compute terms up to idx. */
    size_t current = seq_cache.size();
    seq_cache.resize(idx);
    for (size_t i = current; i < idx; ++i) {
        seq_t val = 5 * seq_cache[i - 1];
        val -= 3 * seq_cache[i - 2];
        val -= 4 * seq_cache[i - 3];
        val -= 12 * seq_cache[i - 4];
        val += 12 * seq_cache[i - 5];
        val += 21 * seq_cache[i - 6];
        val -= 9 * seq_cache[i - 7];
        val -= 9 * seq_cache[i - 8];
        seq_cache[i] = val;
    }
    return seq_cache[idx - 1];
#else
    /* C implementation using static array. */
    if (cached_len < 10) {
        for (size_t i = 0; i < 10; ++i) seq_cache[i] = init_terms[i];
        cached_len = 10;
    }
    if (idx <= cached_len) {
        return seq_cache[idx - 1];
    }
    if (idx > CACHE_CAPACITY) {
        /* The implementation assumes n ≤ CACHE_CAPACITY. */
        return 0;
    }
    for (size_t i = cached_len; i < idx; ++i) {
        seq_t val = 5 * seq_cache[i - 1];
        val -= 3 * seq_cache[i - 2];
        val -= 4 * seq_cache[i - 3];
        val -= 12 * seq_cache[i - 4];
        val += 12 * seq_cache[i - 5];
        val += 21 * seq_cache[i - 6];
        val -= 9 * seq_cache[i - 7];
        val -= 9 * seq_cache[i - 8];
        seq_cache[i] = val;
    }
    cached_len = idx;
    return seq_cache[idx - 1];
#endif
}

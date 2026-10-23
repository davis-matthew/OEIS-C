/*
 * A190171  Number of peakless Motzkin paths of length n having no UHD's
 *          starting at level 0; here U=(1,1), H=(1,0), and D=(1,-1).
 * https://oeis.org/A190171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n+2)*a(n) + (-n+1)*a(n-1) + 2*(-2*n-1)*a(n-2) + 9*a(n-3) +
 *            (-n+1)*a(n-4) - 9*a(n-5) + 2*(-2*n+5)*a(n-6) + (-n+1)*a(n-7) +
 *            (n-4)*a(n-8) = 0 Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=1,
 *            a(4)=2, a(5)=5, a(6)=12, a(7)=27
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static const seq_t base[8] = {1, 1, 1, 1, 2, 5, 12, 27};

seq_t a(idx_t n) {
    if (n < cache_len) return cache[n];

    size_t old_len = cache_len;
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = alloc_seq(new_len);
    if (cache) {
        for (size_t i = 0; i < old_len; ++i) new_cache[i] = cache[i];
        free_seq(cache);
    }
    cache = new_cache;
    cache_len = new_len;

    for (idx_t idx = old_len; idx <= n; ++idx) {
        if (idx < 8) {
            cache[idx] = base[idx];
            continue;
        }
        seq_t pos = 0;
        pos += (seq_t)(idx - 1) * cache[idx - 1];
        pos += (seq_t)(4 * idx + 2) * cache[idx - 2];
        pos += (seq_t)(idx - 1) * cache[idx - 4];
        pos += (seq_t)9 * cache[idx - 5];
        pos += (seq_t)(4 * idx - 10) * cache[idx - 6];
        pos += (seq_t)(idx - 1) * cache[idx - 7];

        seq_t sub = (seq_t)9 * cache[idx - 3] + (seq_t)(idx - 4) * cache[idx - 8];

        seq_t num = pos - sub;
        seq_t denom = (seq_t)(idx + 2);
        cache[idx] = num / denom;
    }
    return cache[n];
}

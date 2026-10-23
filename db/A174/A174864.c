/*
 * A174864  a(1) = 1, a(n) = square of the sum of previous terms.
 * https://oeis.org/A174864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 1; for n > 1, a(n) = (sum_{k=1}^{n-1} a(k))^2
 * The function memoises previously computed terms so that
 * calls can be in arbitrary order and each call is O(n).
 */
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static seq_t cur_sum = 0;

    while (cache.size() < (size_t)n) {
        if (cache.empty()) {
            cache.push_back((seq_t)1);
            cur_sum = 1;
        } else {
            seq_t next = cur_sum * cur_sum;
            cache.push_back(next);
            cur_sum += next;
        }
    }
    return cache[(size_t)n - 1];
#else
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t cur_sum = 0;

    /* ensure memory for n terms */
    if (cache_len < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;

        while (cache_len < (size_t)n) {
            if (cache_len == 0) {
                cache[cache_len++] = (seq_t)1;
                cur_sum = 1;
            } else {
                seq_t next = cur_sum * cur_sum;
                cache[cache_len++] = next;
                cur_sum += next;
            }
        }
    }
    return cache[(size_t)n - 1];
#endif
}

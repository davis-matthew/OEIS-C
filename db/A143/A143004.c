/*
 * A143004  a(0) = 0, a(1) = 1, a(n+1) = (2*n+1)*(n^2+n+13)*a(n) - n^6*a(n-1).
 * https://oeis.org/A143004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 1,
   a(n+1) = (2*n+1)*(n^2+n+13)*a(n) - n^6*a(n-1)
   for n >= 1 */
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    static idx_t cache_len = 0;

#ifdef __cplusplus
    /* Use std::vector for C++ builds */
    static std::vector<seq_t> cache_vec;
    if (n + 1 > cache_len) {
        cache_vec.resize(n + 1);
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache_vec[i] = 0;
            }
            else if (i == 1) {
                cache_vec[i] = 1;
            }
            else {
                idx_t n_prev_idx = i - 1;
                seq_t sn = (seq_t)n_prev_idx;
                seq_t factor = (seq_t)2 * sn + 1;
                seq_t sq = sn * sn;
                seq_t part = sq + sn + (seq_t)13;
                seq_t term1 = factor * part * cache_vec[i - 1];
                seq_t n6 = sn;
                for (int j = 0; j < 5; ++j) n6 = n6 * sn;   /* sn^6 */
                seq_t term2 = n6 * cache_vec[i - 2];
                cache_vec[i] = term1 - term2;
            }
        }
        cache_len = (idx_t)(n + 1);
    }
    return cache_vec[n];
#else
    /* Use malloc/realloc for C builds */
    static seq_t *cache = NULL;
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 0;
            }
            else if (i == 1) {
                cache[i] = 1;
            }
            else {
                idx_t n_prev_idx = i - 1;
                seq_t sn = (seq_t)n_prev_idx;
                seq_t factor = (seq_t)2 * sn + 1;
                seq_t sq = sn * sn;
                seq_t part = sq + sn + (seq_t)13;
                seq_t term1 = factor * part * cache[i - 1];
                seq_t n6 = sn;
                for (int j = 0; j < 5; ++j) n6 = n6 * sn;   /* sn^6 */
                seq_t term2 = n6 * cache[i - 2];
                cache[i] = term1 - term2;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
#endif
}

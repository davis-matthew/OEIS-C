/*
 * A063820  Growth series for fundamental group of orientable closed surface
 *          of genus 10.
 * https://oeis.org/A063820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=40. For 0 < n < 20: a(n)=39*a(n-1). For n = 20:
 *            a(20)=39*a(19)-39*a(0)-1. For n = 21:
 *            a(21)=39*a(20)-39*a(1)+a(0)-1. For n ≥ 22:
 *            a(n)=39*a(n-1)-39*a(n-20)+a(n-21)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated size of cache */

    size_t need = (size_t)n + 1;
    if (need > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!new_cache && need > 0)
            return 0;  /* allocation failed – unlikely in tests */
        cache = new_cache;

        for (size_t i = cache_size; i < need; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)39 * cache[0] + (seq_t)1;   /* 39*1+1 = 40 */
            } else if (i <= 19) {
                cache[i] = (seq_t)39 * cache[i-1];
            } else if (i == 20) {
                cache[20] = (seq_t)39 * cache[19] - (seq_t)39 * cache[0] - (seq_t)1;
            } else if (i == 21) {
                cache[21] = (seq_t)39 * cache[20] - (seq_t)39 * cache[1] + cache[0] - (seq_t)1;
            } else {
                cache[i] = (seq_t)39 * cache[i-1]
                         - (seq_t)39 * cache[i-20]
                         + cache[i-21];
            }
        }
        cache_size = need;
    }
    return cache[n];
}

/*
 * A004112  Numbers k where |cos(k)| (or |cosec(k)| or |cot(k)|) decreases
 *          monotonically to 0; also numbers k where |tan(k)| (or |sec(k)|, or
 *          |sin(k)|) increases.
 * https://oeis.org/A004112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds integers k where k mod π approaches π/2 monotonically, giving the sequence of angles where |cos(k)| → 0 */

#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979323846264338327950288L

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t num_terms = 0;
    static long double min_dist;
    static unsigned long long k;
    
    if (cache == NULL) {
        cache_len = (n < 100) ? 200 : (n + 100);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        min_dist = PI / 2;
        cache[0] = 0;
        num_terms = 1;
        k = 1;
    } else if (n > cache_len) {
        while (n > cache_len) cache_len *= 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_len);
    }
    
    while (num_terms < n) {
        long double r = fmodl((long double)k, PI);
        long double dist = fabsl(r - PI / 2);
        
        if (dist < min_dist - 1e-16) {
            cache[num_terms++] = (seq_t)k;
            min_dist = dist;
        }
        k++;
    }
    
    return cache[n - 1];
}

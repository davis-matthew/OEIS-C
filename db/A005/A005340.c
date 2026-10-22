/*
 * A005340  Number of ways in which n identical balls can be distributed among
 *          7 boxes in a row such that each pair of adjacent boxes contains at
 *          least 4 balls.
 * https://oeis.org/A005340
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: x^12*(1 + 6*x + 6*x^2 + x^3 - 66*x^5 + 74*x^6 -
 *            21*x^7)/(1-x)^7 Recurrence: a(n) = 7*a(n-1) - 21*a(n-2) +
 *            35*a(n-3) - 35*a(n-4) + 21*a(n-5) - 7*a(n-6) + a(n-7) + c(n)
 *            where c(n) is [x^{n-12}](1 + 6*x + 6*x^2 + x^3 - 66*x^5 + 74*x^6
 *            - 21*x^7)
 * Range:     u8 n <= 14, u16 n <= 23, u32 n <= 119, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define CACHE_SIZE 10001

static seq_t cache[CACHE_SIZE];
static idx_t cache_max = 0;
static int initialized = 0;

seq_t a(idx_t n) {
    if (n < 12) return 0;
    
    idx_t idx = n - 12;
    if (idx >= CACHE_SIZE) return 0;
    
    if (!initialized) {
        cache[0] = 1;
        cache_max = 0;
        initialized = 1;
    }
    
    if (idx > cache_max) {
        for (idx_t i = cache_max + 1; i <= idx; i++) {
            idx_t n_val = i + 12;
            seq_t pos = 0, neg = 0;
            
            if (i >= 1) pos += 7 * cache[i-1];
            if (i >= 2) neg += 21 * cache[i-2];
            if (i >= 3) pos += 35 * cache[i-3];
            if (i >= 4) neg += 35 * cache[i-4];
            if (i >= 5) pos += 21 * cache[i-5];
            if (i >= 6) neg += 7 * cache[i-6];
            if (i >= 7) pos += cache[i-7];
            
            if (n_val == 12) pos += 1;
            else if (n_val == 13) pos += 6;
            else if (n_val == 14) pos += 6;
            else if (n_val == 15) pos += 1;
            else if (n_val == 17) neg += 66;
            else if (n_val == 18) pos += 74;
            else if (n_val == 19) neg += 21;
            
            cache[i] = pos - neg;
        }
        cache_max = idx;
    }
    
    return cache[idx];
}

/*
 * A005933  Theta series of laminated lattice LAMBDA_9.
 * https://oeis.org/A005933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of q^n in theta_3(q)^9 -
 *            theta_3(q)^5*theta_4(q)^4 + (theta_3(q)*theta_4(q)^8 +
 *            theta_4(q)^9)/2 where theta_3(q) = Sum_{k in Z} q^(k^2) and
 *            theta_4(q) = Sum_{k in Z} (-1)^k*q^(k^2)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 231, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 51 terms through n = 9894 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/mem.h"

static void multiply_series_i64(int64_t *result, const int64_t *a, const int64_t *b, idx_t len) {
    int64_t *temp = (int64_t *)calloc(len, sizeof(int64_t));
    
    for (idx_t i = 0; i < len; i++) {
        if (!a[i]) continue;
        for (idx_t j = 0; j < len - i; j++) {
            if (!b[j]) continue;
            temp[i + j] += a[i] * b[j];
        }
    }
    
    memcpy(result, temp, len * sizeof(int64_t));
    free(temp);
}

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void compute_cache(idx_t target_len) {
    int64_t *theta3 = (int64_t *)calloc(target_len, sizeof(int64_t));
    int64_t *theta4 = (int64_t *)calloc(target_len, sizeof(int64_t));
    
    theta3[0] = 1;
    for (idx_t k = 1; k * k < target_len; k++) {
        theta3[k * k] = 2;
    }
    
    theta4[0] = 1;
    for (idx_t k = 1; k * k < target_len; k++) {
        theta4[k * k] = (k % 2 == 1) ? -2 : 2;
    }
    
    int64_t *t3_5 = (int64_t *)calloc(target_len, sizeof(int64_t));
    memcpy(t3_5, theta3, target_len * sizeof(int64_t));
    for (int i = 1; i < 5; i++) {
        int64_t *temp = (int64_t *)calloc(target_len, sizeof(int64_t));
        multiply_series_i64(temp, t3_5, theta3, target_len);
        free(t3_5);
        t3_5 = temp;
    }
    
    int64_t *t3_9 = (int64_t *)calloc(target_len, sizeof(int64_t));
    memcpy(t3_9, theta3, target_len * sizeof(int64_t));
    for (int i = 1; i < 9; i++) {
        int64_t *temp = (int64_t *)calloc(target_len, sizeof(int64_t));
        multiply_series_i64(temp, t3_9, theta3, target_len);
        free(t3_9);
        t3_9 = temp;
    }
    
    int64_t *t4_4 = (int64_t *)calloc(target_len, sizeof(int64_t));
    memcpy(t4_4, theta4, target_len * sizeof(int64_t));
    for (int i = 1; i < 4; i++) {
        int64_t *temp = (int64_t *)calloc(target_len, sizeof(int64_t));
        multiply_series_i64(temp, t4_4, theta4, target_len);
        free(t4_4);
        t4_4 = temp;
    }
    
    int64_t *t4_8 = (int64_t *)calloc(target_len, sizeof(int64_t));
    memcpy(t4_8, theta4, target_len * sizeof(int64_t));
    for (int i = 1; i < 8; i++) {
        int64_t *temp = (int64_t *)calloc(target_len, sizeof(int64_t));
        multiply_series_i64(temp, t4_8, theta4, target_len);
        free(t4_8);
        t4_8 = temp;
    }
    
    int64_t *t4_9 = (int64_t *)calloc(target_len, sizeof(int64_t));
    memcpy(t4_9, theta4, target_len * sizeof(int64_t));
    for (int i = 1; i < 9; i++) {
        int64_t *temp = (int64_t *)calloc(target_len, sizeof(int64_t));
        multiply_series_i64(temp, t4_9, theta4, target_len);
        free(t4_9);
        t4_9 = temp;
    }
    
    int64_t *t3_5_t4_4 = (int64_t *)calloc(target_len, sizeof(int64_t));
    multiply_series_i64(t3_5_t4_4, t3_5, t4_4, target_len);
    
    int64_t *t3_t4_8 = (int64_t *)calloc(target_len, sizeof(int64_t));
    multiply_series_i64(t3_t4_8, theta3, t4_8, target_len);
    
    if (cache) mem_free(cache);
    cache = mem_new(seq_t, target_len);
    cache_len = target_len;
    
    for (idx_t i = 0; i < target_len; i++) {
        int64_t val = t3_9[i] - t3_5_t4_4[i];
        int64_t sum = t3_t4_8[i] + t4_9[i];
        val += sum / 2;
        cache[i] = (seq_t)val;
    }
    
    free(theta3);
    free(theta4);
    free(t3_5);
    free(t3_9);
    free(t4_4);
    free(t4_8);
    free(t4_9);
    free(t3_5_t4_4);
    free(t3_t4_8);
}

seq_t a(idx_t n) {
    if (n >= cache_len) {
        compute_cache(n + 1);
    }
    return cache[n];
}

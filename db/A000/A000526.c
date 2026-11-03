/*
 * A000526  Number of partially labeled trees with n nodes (5 of which are
 *          labeled).
 * https://oeis.org/A000526
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] in
 *            B(x)^5*(125-204*B(x)+118*B(x)^2-24*B(x)^3)/(1-B(x))^7 where B(x)
 *            is the generating function for rooted trees (A000081)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 73,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 194 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t* poly_mult(const seq_t *p, const seq_t *q, long long N) {
    seq_t *result = mem_new(seq_t, (N + 1));
    mem_set(result, 0, (N + 1) * sizeof(seq_t));
    for (long long i = 0; i <= N; i++) {
        if (p[i] == 0) continue;
        for (long long j = 0; i + j <= N; j++) {
            result[i + j] += p[i] * q[j];
        }
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 5) return 0;
    
    long long N = (long long)n;
    
    seq_t *b = mem_new(seq_t, N + 1);
    b[1] = 1;
    
    for (long long i = 2; i <= N; i++) {
        seq_t sum = 0;
        for (long long k = 1; k < i; k++) {
            seq_t sigma_k = 0;
            for (long long d = 1; d * d <= k; d++) {
                if (k % d == 0) {
                    sigma_k += (seq_t)d * b[d];
                    if (d != k / d) {
                        sigma_k += (seq_t)(k / d) * b[k / d];
                    }
                }
            }
            sum += sigma_k * b[i - k];
        }
        b[i] = sum / (seq_t)(i - 1);
    }
    
    seq_t *b2 = poly_mult(b, b, N);
    seq_t *b3 = poly_mult(b2, b, N);
    seq_t *b4 = poly_mult(b2, b2, N);
    seq_t *b5 = poly_mult(b4, b, N);
    
    seq_t *poly_inner = mem_new(seq_t, (N + 1));
    mem_set(poly_inner, 0, (N + 1) * sizeof(seq_t));
    poly_inner[0] = 125;
    for (long long i = 0; i <= N; i++) {
        poly_inner[i] -= 204 * b[i];
        poly_inner[i] += 118 * b2[i];
        poly_inner[i] -= 24 * b3[i];
    }
    
    seq_t *numerator = poly_mult(b5, poly_inner, N);
    
    seq_t *inv_denom = mem_new(seq_t, N + 1);
    inv_denom[0] = 1;
    
    seq_t *bm = mem_new(seq_t, (N + 1));
    mem_copy(bm, b, (N + 1) * sizeof(seq_t));
    
    for (long long m = 1; m <= N; m++) {
        seq_t binom = ((seq_t)(m + 1) * (seq_t)(m + 2) * (seq_t)(m + 3) * 
                       (seq_t)(m + 4) * (seq_t)(m + 5) * (seq_t)(m + 6)) / 720;
        
        for (long long i = 0; i <= N; i++) {
            inv_denom[i] += binom * bm[i];
        }
        
        if (m < N) {
            seq_t *bm_new = poly_mult(bm, b, N);
            mem_free(bm);
            bm = bm_new;
        }
    }
    
    seq_t *result_poly = poly_mult(numerator, inv_denom, N);
    seq_t result = result_poly[N];
    
    mem_free(b);
    mem_free(b2);
    mem_free(b3);
    mem_free(b4);
    mem_free(b5);
    mem_free(poly_inner);
    mem_free(numerator);
    mem_free(inv_denom);
    mem_free(bm);
    mem_free(result_poly);
    
    return result;
}

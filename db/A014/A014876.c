/*
 * A014876  Numbers k that divide s(k), where s(1)=1, s(j)=25*s(j-1)+j.
 * https://oeis.org/A014876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 370, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A014876: Numbers k such that k divides s(k), where
 * s(1)=1, s(n)=25*s(n-1)+n. The function returns the n-th such k. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static const uint64_t A00 = 25, A01 = 1, A02 = 1;
static const uint64_t A10 = 0, A11 = 1, A12 = 1;
static const uint64_t A20 = 0, A21 = 0, A22 = 1;

typedef struct {
    uint64_t m[3][3];
} Mat;

/* Multiply matrices A and B modulo mod. */
static inline void mat_mul(Mat *res, const Mat *a, const Mat *b, uint64_t mod) {
    Mat c;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            __uint128_t sum = 0;
            for (int k = 0; k < 3; ++k) {
                sum += (__uint128_t)a->m[i][k] * b->m[k][j];
            }
            c.m[i][j] = (uint64_t)(sum % mod);
        }
    }
    *res = c;
}

/* Binary exponentiation of matrix base to power exp modulo mod. */
static inline Mat mat_pow(uint64_t base[3][3], uint64_t exp, uint64_t mod) {
    Mat result = { .m = { {1,0,0},{0,1,0},{0,0,1} } };
    Mat base_mat = { .m = { {base[0][0], base[0][1], base[0][2]},
                           {base[1][0], base[1][1], base[1][2]},
                           {base[2][0], base[2][1], base[2][2]} } };
    while (exp) {
        if (exp & 1ULL) mat_mul(&result, &result, &base_mat, mod);
        mat_mul(&base_mat, &base_mat, &base_mat, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* Check if k divides s(k). */
static inline bool divides(uint64_t k) {
    if (k == 1) return true;   /* s(1)=1, 1 divides 1 */
    /* compute A^(k-1) modulo k and apply to vector [1,1,1]^T */
    uint64_t init_matrix[3][3] = { { A00, A01, A02 },
                                   { A10, A11, A12 },
                                   { A20, A21, A22 } };
    Mat M = mat_pow(init_matrix, k - 1, k);
    /* t_k = M[0]*[1,1,1]^T = M00+M01+M02 (mod k) */
    uint64_t t = (M.m[0][0] + M.m[0][1] + M.m[0][2]) % k;
    return t == 0;
}

/* Dynamic cache of found sequence terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;

/* Ensure cache has capacity for at least one more element. */
static void grow_cache(void) {
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n) {
    /* return the n-th term (1‑based). */
    if (n == 0) {  /* undefined, but guard */
        return (seq_t)0;
    }
    while (cache_len < (size_t)n) {
        if (divides(next_candidate)) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}

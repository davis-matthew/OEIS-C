/*
 * A165937  G.f.: A(x) = exp( Sum_{n>=1} A002203(n^2)*x^n/n ).
 * https://oeis.org/A165937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 1/n * sum_{k=1}^n A002203(k^2) * a(n-k),
 * where A002203(m) = (1+sqrt(2))^m + (1-sqrt(2))^m.
 *
 * The implementation computes the A002203 values via fast matrix exponentiation
 * and then uses the above recurrence to obtain a(n).
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Compute A002203(m) = (1+sqrt(2))^m + (1-sqrt(2))^m
 * using the recurrence a_m = 2*a_{m-1} + a_{m-2}.
 * This is done by exponentiation of the companion matrix. */
static seq_t pell(uint64_t m)
{
    if (m == 0)
        return (seq_t)2;
    if (m == 1)
        return (seq_t)2;

    seq_t R[2][2] = { { (seq_t)1, (seq_t)0 },
                      { (seq_t)0, (seq_t)1 } };          /* identity */
    seq_t M[2][2] = { { (seq_t)2, (seq_t)1 },
                      { (seq_t)1, (seq_t)0 } };

    uint64_t e = m - 1;   /* exponent for M^(m-1) */

    while (e) {
        if (e & 1) {
            seq_t tmp00 = R[0][0] * M[0][0] + R[0][1] * M[1][0];
            seq_t tmp01 = R[0][0] * M[0][1] + R[0][1] * M[1][1];
            seq_t tmp10 = R[1][0] * M[0][0] + R[1][1] * M[1][0];
            seq_t tmp11 = R[1][0] * M[0][1] + R[1][1] * M[1][1];
            R[0][0] = tmp00; R[0][1] = tmp01;
            R[1][0] = tmp10; R[1][1] = tmp11;
        }
        seq_t tmp00 = M[0][0] * M[0][0] + M[0][1] * M[1][0];
        seq_t tmp01 = M[0][0] * M[0][1] + M[0][1] * M[1][1];
        seq_t tmp10 = M[1][0] * M[0][0] + M[1][1] * M[1][0];
        seq_t tmp11 = M[1][0] * M[0][1] + M[1][1] * M[1][1];
        M[0][0] = tmp00; M[0][1] = tmp01;
        M[1][0] = tmp10; M[1][1] = tmp11;
        e >>= 1;
    }

    /* Multiply the resulting matrix with the initial vector [a1, a0] = [2, 2] */
    seq_t a1 = (seq_t)2;
    seq_t a0 = (seq_t)2;
    return R[0][0] * a1 + R[0][1] * a0;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static seq_t *B = NULL;
    static idx_t B_len = 0;

    /* Ensure cache[0] is initialized */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)
            return (seq_t)0;
        cache[0] = (seq_t)1;
    }

    if (n == 0)
        return cache[0];   /* a(0) = 1 */

    /* Grow cache array if necessary */
    if (cache_len <= n) {
        idx_t new_len = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;
        memset(tmp + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        cache = tmp;
        cache_len = new_len;
    }

    /* Grow B array if necessary */
    if (B_len <= n) {
        idx_t new_len = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(B, new_len * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;
        memset(tmp + B_len, 0, (new_len - B_len) * sizeof(seq_t));
        B = tmp;
        B_len = new_len;
    }

    /* Compute B values for k = 1 .. n, where B[k] = A002203(k^2) */
    for (idx_t k = 1; k <= n; ++k) {
        if (B[k] == 0) {
            B[k] = pell((unsigned long long)k * (unsigned long long)k);
        }
    }

    /* Recurrence: a_i = (1/i) * sum_{k=1}^i B[k] * a_{i-k} */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] == 0) {           /* Not yet computed */
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += B[k] * cache[i - k];
            }
            cache[i] = sum / i;        /* division is exact */
        }
    }

    return cache[n];
}

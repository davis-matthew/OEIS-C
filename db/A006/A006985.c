/*
 * A006985  Fibonacci tower: a(n) = F(a(n-1)+2) (there is no room for next
 *          term).
 * https://oeis.org/A006985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = F(a(n-1)+2), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static void mat_mul(seq_t a[2][2], seq_t b[2][2], seq_t c[2][2]) {
    seq_t temp[2][2];
    temp[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0];
    temp[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1];
    temp[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0];
    temp[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1];
    
    c[0][0] = temp[0][0];
    c[0][1] = temp[0][1];
    c[1][0] = temp[1][0];
    c[1][1] = temp[1][1];
}

static void mat_pow(seq_t n, seq_t result[2][2]) {
    result[0][0] = 1; result[0][1] = 0;
    result[1][0] = 0; result[1][1] = 1;
    
    seq_t base[2][2] = {{1, 1}, {1, 0}};
    
    while (n > 0) {
        if (n & 1) {
            seq_t temp[2][2];
            mat_mul(result, base, temp);
            result[0][0] = temp[0][0]; result[0][1] = temp[0][1];
            result[1][0] = temp[1][0]; result[1][1] = temp[1][1];
        }
        seq_t temp[2][2];
        mat_mul(base, base, temp);
        base[0][0] = temp[0][0]; base[0][1] = temp[0][1];
        base[1][0] = temp[1][0]; base[1][1] = temp[1][1];
        n >>= 1;
    }
}

static seq_t compute_fib(seq_t m) {
    seq_t result[2][2];
    mat_pow(m, result);
    return result[0][1];
}

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_len = n + 10;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        memset(cache, 0, cache_len * sizeof(seq_t));
    } else if (cache_len <= n) {
        idx_t new_len = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        memset(new_cache + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    if (n == 0) {
        cache[0] = 1;
        return 1;
    }
    
    if (cache[n] != 0) {
        return cache[n];
    }
    
    seq_t prev = a(n - 1);
    seq_t fib_idx = prev + 2;
    seq_t result = compute_fib(fib_idx);
    cache[n] = result;
    return result;
}

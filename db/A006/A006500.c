/*
 * A006500  Restricted combinations.
 * https://oeis.org/A006500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(floor(n/3) + 3)^(n mod 3)*F(floor(n/3) + 2)^(3 - (n mod
 *            3)) where F(n) is Fibonacci
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fib_cache = NULL;
static idx_t fib_cache_len = 0;

static seq_t fib(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    if (n >= fib_cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache) {
            seq_t a = 0, b = 1;
            for (idx_t i = 2; i <= n; i++) {
                seq_t c = a + b;
                a = b;
                b = c;
            }
            return b;
        }
        
        if (fib_cache) {
            for (idx_t i = 0; i < fib_cache_len; i++) {
                new_cache[i] = fib_cache[i];
            }
            free(fib_cache);
        } else {
            new_cache[0] = 0;
            new_cache[1] = 1;
        }
        
        idx_t start = (fib_cache_len > 1) ? fib_cache_len : 2;
        for (idx_t i = start; i <= n; i++) {
            new_cache[i] = new_cache[i-1] + new_cache[i-2];
        }
        
        fib_cache = new_cache;
        fib_cache_len = new_len;
    }
    
    return fib_cache[n];
}

static seq_t power(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    idx_t k = n / 3;
    idx_t r = n % 3;
    
    seq_t f_k2 = fib(k + 2);
    seq_t f_k3 = fib(k + 3);
    
    seq_t result = power(f_k3, r) * power(f_k2, 3 - r);
    return result;
}

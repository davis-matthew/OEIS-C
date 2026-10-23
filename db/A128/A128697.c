/*
 * A128697  Sum of the eighth powers of the first n Fibonacci numbers.
 * https://oeis.org/A128697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} F(k)^8 (F(k) = Fibonacci number)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   pow8_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow8_func.h"

/* External linkage function that returns term a(n) */
seq_t a(idx_t n) {
    /* Static state to memoize computed values */
    static seq_t *cache = NULL;           /* array of a(k) values   */
    static size_t cache_cap = 0;          /* allocated array size */
    static size_t cache_len = 0;          /* number of terms stored (>=1) */
    static seq_t cum_sum = 0;             /* a(cache_len-1) */
    static seq_t fib_prev = 0;            /* Fibonacci(k-1) */
    static seq_t fib_curr = 1;            /* Fibonacci(k)   */
    
    /* Initialise on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)0;         /* a(0) = 0 */
        cache_len = 1;               /* have term 0 stored */
    }
    
    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[(size_t)n];
    
    /* Extend cache up to n, computing Fibonacci and powers incrementally */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t pow8 = pow8_func(fib_curr);  /* F(i)^8  (fib_curr == F(i)) */
        cum_sum = cum_sum + pow8;          /* a(i) = a(i-1) + F(i)^8 */
        
        /* Grow array if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            cache_cap = new_cap;
        }
        cache[cache_len] = cum_sum;
        ++cache_len;
        
        /* Advance Fibonacci numbers */
        seq_t next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = next;
    }
    
    return cache[(size_t)n];
}

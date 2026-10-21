/*
 * A052959  a(2n) = a(2n-1)+a(2n-2), a(2n+1) = a(2n)+a(2n-1)-1, a(0)=2,
 *          a(1)=1.
 * https://oeis.org/A052959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fib(n+1) + (1+(-1)^n)/2 Recurrence used: a(0)=2, a(1)=1,
 *            a(2)=3; for n ≥ 3: a(n) = 2*a(n-2) + a(n-3) - 1
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Adapted to any seq_t/idx_t types that support +, -, and + with integer constants.
#include <stdlib.h>

static seq_t *cache = NULL;    // store computed values
static idx_t cache_cap = 0;    // allocated capacity
static idx_t cache_len = 0;    // number of terms currently stored

// Extend cache up to `n`, computing new terms as needed.
static void ensure_cache(idx_t n)
{
    // Grow array until it can hold index n.
    while (cache_len <= n) {
        if (cache_len == 0) {
            cache_cap = 4;
            cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }

        seq_t val;
        idx_t i = cache_len;
        if (i == 0)          val = (seq_t)2;
        else if (i == 1)     val = (seq_t)1;
        else if (i == 2)     val = (seq_t)3;
        else {
            // val = 2*cache[i-2] + cache[i-3] - 1
            val = cache[i-2] + cache[i-2];   // 2*cache[i-2]
            val += cache[i-3];
            val -= (seq_t)1;
        }
        cache[cache_len++] = val;
    }
}

seq_t a(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 0;
        cache_len  = 0;
    }
    ensure_cache(n);
    return cache[n];
}

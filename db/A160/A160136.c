/*
 * A160136  Lodumo_9 of Fibonacci numbers.
 * https://oeis.org/A160136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lod_9(Fibonacci(n))
 * Range:     u8 n <= 133, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = 9*count[r] + r where r = Fibonacci(n) mod 9
 *  and count[r] is the number of previous occurrences of residue r
 *  (before incrementing for the current n). */

#include <stdlib.h>
#include <stdint.h>

/* Cached state (static for all calls) */
static seq_t *cache           = NULL;          /* a(n) values */
static uint8_t *fib_cache     = NULL;          /* Fibonacci(n) mod 9 */
static idx_t  cached_len      = 0;             /* number of filled entries in cache */
static idx_t  cached_cap      = 0;             /* allocated capacity of cache */
static idx_t  max_n           = 0;             /* highest n that has been computed */
static idx_t  counts[9]       = {0,0,0,0,0,0,0,0,0};

seq_t a(idx_t n)
{
    /*--- First call or request greater than cached range ---*/
    if (n >= cached_len) {
        /* New capacity: at least n+1, grow geometrically for future calls */
        idx_t new_cap = cached_cap == 0 ? 16 : cached_cap;
        while (new_cap <= n) new_cap <<= 1;

        /* Allocate or resize cache array */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* Allocate or resize Fibonacci-mod9 cache array */
        fib_cache = (uint8_t *)realloc(fib_cache, new_cap * sizeof(uint8_t));
        cached_cap = new_cap;

        /* Pre-fill missing entries */
        if (cached_len == 0) {
            /* Initialize first term (n = 0) */
            uint8_t r = 0;                         /* Fibonacci(0) mod 9 */
            fib_cache[0] = r;
            cache[0] = 9 * (seq_t)counts[r] + r;   /* counts[r] == 0 */
            counts[r]++;
            cached_len = 1;
            max_n = 0;

            /* Handle second term if needed */
            if (n >= 1) {
                r = 1;                             /* Fibonacci(1) mod 9 */
                fib_cache[1] = r;
                cache[1] = 9 * (seq_t)counts[r] + r; /* counts[r] == 0 */
                counts[r]++;
                cached_len = 2;
                max_n = 1;
            }
        }

        /* Extend from current max_n+1 up to requested n */
        for (idx_t idx = max_n + 1; idx <= n; ++idx) {
            uint8_t r;
            if (idx == 1) {
                r = 1;  /* Fibonacci(1) mod 9 */
            } else if (idx == 0) {
                r = 0;  /* Fibonacci(0) mod 9 */
            } else {
                /* Compute Fibonacci mod 9 using previous two values */
                r = (fib_cache[idx - 1] + fib_cache[idx - 2]) % 9;
            }
            fib_cache[idx] = r;
            cache[idx] = 9 * (seq_t)counts[r] + r;
            counts[r]++;
        }

        max_n = n;
        if (cached_len <= n) cached_len = n + 1;
    }

    /*--- Cached value available, return it ---*/
    return cache[n];
}

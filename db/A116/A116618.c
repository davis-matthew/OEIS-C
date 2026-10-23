/*
 * A116618  Values of Fibonacci(n) for n such that prime(2n) mod 12 = 1.
 * https://oeis.org/A116618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) where prime(2n) mod 12 = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 14, u64 n <= 26, u128 n <= 44,
 *            big any n
 * Helpers:   fib, get_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/fib.h"
#include "../utils/get_prime.h"
/* seq_alloc and seq_realloc are provided for dynamic arrays of seq_t */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache     = NULL;   /* cached terms of the sequence   */
static size_t cache_len = 0;      /* number of terms stored         */
static size_t cache_cap = 0;      /* current capacity of the cache  */
static idx_t  last_tested = 0;    /* highest Fibonacci index checked */

/* Ensure cache has enough capacity; return pointer to new cache if resized */
static void ensure_cache_capacity(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* out of offset, return 0 */

    /* Lazily allocate the cache */
    if (!cache) {
        cache_cap = 16;
        cache     = seq_alloc(cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        last_tested++;                      /* next Fibonacci index to test */

        /* Get the 2*last_tested-th prime */
        uint32_t prime_val;
        prime_val = get_prime(2 * last_tested);

        if (prime_val % 12U == 1U) {
            /* Condition satisfied – store Fibonacci(last_tested) */
            seq_t val = fib(last_tested);

            ensure_cache_capacity();
            cache[cache_len++] = val;
        }
    }

    return cache[n - 1];
}

/*
 * A137494  Fibonacci numbers where no digit is a prime.
 * https://oeis.org/A137494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A137494: a(n) = Fibonacci number whose decimal digits contain no prime digits (2,3,5,7) */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Return true if any decimal digit of x is 2, 3, 5, or 7. */
static bool has_prime_digit(seq_t x)
{
    if (x == (seq_t)0)
        return false;
    while (x != (seq_t)0) {
        uint64_t d = (uint64_t)(x % (seq_t)10);
        if (d == 2 || d == 3 || d == 5 || d == 7)
            return true;
        x = x / (seq_t)10;
    }
    return false;
}

/* Returns the n-th Fibonacci number whose decimal representation contains
 * none of the digits 2, 3, 5, or 7.  Indexing starts at n = 1:
 * a(1) = 0, a(2) = 1, a(3) = 1, a(4) = 8, … */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    /* static state for memoisation and incremental Fibonacci generation */
    static seq_t *cache = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static seq_t f_prev = (seq_t)0;   /* current Fibonacci value (F_k) */
    static seq_t f_next = (seq_t)1;   /* next Fibonacci value               */
    static bool initialized = false;

    if (!initialized) {
        f_prev = (seq_t)0;
        f_next = (seq_t)1;
        cache_len = 0;
        cache_cap = 0;
        cache = NULL;
        initialized = true;
    }

    /* generate Fibonacci numbers until we have at least n valid terms */
    while (cache_len < (size_t)n) {
        /* current Fibonacci value is f_prev */
        if (!has_prime_digit(f_prev)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {                 /* allocation failure – return 0 */
                    free(cache);
                    cache = NULL;
                    cache_len = cache_cap = 0;
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = f_prev;
        }

        /* advance the Fibonacci pair: F_k -> F_{k+1} -> F_{k+2} */
        seq_t next_val = f_prev + f_next;
        f_prev = f_next;
        f_next = next_val;
    }

    return cache[(size_t)n - 1];
}

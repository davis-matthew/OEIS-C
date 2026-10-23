/*
 * A122825  a(n) = n + number of previous prime terms, a(1) = 1.
 * https://oeis.org/A122825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (isprime(a(n-1)) ? 2 : 1), a(1) = 1
 * Range:     u8 n <= 202, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Simple primality test for seq_t values.
   Works for any unsigned integer type without overflow. */
static inline bool is_prime_seq(seq_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (seq_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0) return 0;          /* undefined for n < 1, but guard */

    if (cache_len == 0) {
        /* Allocate space for the first term */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = (seq_t)1;      /* a(1) = 1 */
        cache_len = 1;
    }

    if (cache_len < n) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        for (idx_t i = cache_len; i < new_len; ++i) {
            seq_t prev = cache[i - 1];
            seq_t d = is_prime_seq(prev) ? (seq_t)2 : (seq_t)1;
            cache[i] = prev + d;
        }
        cache_len = new_len;
    }

    return cache[n - 1];
}

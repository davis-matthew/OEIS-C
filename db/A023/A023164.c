/*
 * A023164  Numbers k such that Fibonacci(k) == -3 (mod k).
 * https://oeis.org/A023164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 507, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A023164 - Numbers k such that Fibonacci(k) == -3 (mod k)
   This implementation searches integers sequentially, checks the
   congruence using fast‑doubling modulo arithmetic, and memoises
   results so multiple calls are efficient. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute F(n) mod m using fast doubling (iterative implementation) */
static inline unsigned long long fib_mod_uint64(idx_t n, unsigned long long m)
{
    unsigned long long a = 0;   /* F0 */
    unsigned long long b = 1;   /* F1 */
    for (int i = 63; i >= 0; --i) {
        /* Calculate F(2k) and F(2k+1) from F(k)=a and F(k+1)=b */
        unsigned long long two_b = (b * 2) % m;
        unsigned long long c = (a * ((two_b + m - a) % m)) % m;           /* F(2k)   */
        unsigned long long d = ((a * a) % m + (b * b) % m) % m;            /* F(2k+1) */

        if ((n >> i) & 1) {            /* bit of n is 1 : go to F(2k+1), F(2k+2) */
            a = d;
            b = (c + d) % m;
        } else {                        /* bit of n is 0 : go to F(2k), F(2k+1) */
            a = c;
            b = d;
        }
    }
    return a;   /* F(n) mod m */
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

/* Append a new element to the cache, expanding as necessary */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();          /* unlikely, but keep safe */
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard against 0 */

    while (cache_len < n) {
        idx_t k = next_candidate;
        unsigned long long fk_mod = fib_mod_uint64(k, (unsigned long long)k);

        /* Condition: Fibonacci(k) == -3 (mod k)  <=>  (fk_mod + 3) % k == 0 */
        if ((fk_mod + 3ULL) % (unsigned long long)k == 0) {
            cache_append((seq_t)k);
        }
        ++next_candidate;
    }

    return cache[n - 1];
}

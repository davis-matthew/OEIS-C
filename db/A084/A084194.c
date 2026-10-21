/*
 * A084194  Even numbers n such that the sum of the digits is prime and the
 *          sum of the digits of n/2 is also prime.
 * https://oeis.org/A084194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 3679, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n‑th even integer k such that digitSum(k) is prime
 *               and digitSum(k/2) is also prime.
 * Implementation: simple search with memoised cache.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* helper: sum of decimal digits of a seq_t value */
static inline uint64_t digit_sum_seq(seq_t v)
{
    uint64_t sum = 0;
    while (v != 0) {
        sum += (uint64_t)(v % 10);
        v /= 10;
    }
    return sum;
}

/* helper: primality test for small unsigned 64‑bit integers */
static inline bool is_prime_small(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1ULL) == 0 || (n % 3ULL) == 0) return false;
    for (uint64_t i = 5; i <= n / i; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n==0, return 0 */

    /* memoised cache of found terms */
    static seq_t *cache   = NULL;   /* array of terms */
    static size_t cache_len = 0;    /* number of terms stored */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* next even candidate to test */
    static seq_t cur = 2;           /* first even integer */

    while (cache_len < n) {
        /* test current candidate */
        if (cur != 0) { /* avoid potential overflow wrap, but cur will never be 0 after start */
            uint64_t s1 = digit_sum_seq(cur);
            if (is_prime_small(s1)) {
                seq_t half = cur / 2;
                uint64_t s2 = digit_sum_seq(half);
                if (is_prime_small(s2)) {
                    /* store term */
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                        if (!tmp) return 0;  /* allocation failure */
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = cur;
                }
            }
        }
        /* advance to next even number */
        cur += 2;
    }

    return cache[n - 1];
}

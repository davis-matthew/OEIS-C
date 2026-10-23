/*
 * A024908  Numbers k such that 9*k - 5 is prime.
 * https://oeis.org/A024908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer k such that 9*k - 5 is prime
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: primality test for unsigned 64-bit integers with trial division. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    if (n <= 3)
        return true;
    if ((n & 1U) == 0 || n % 3ULL == 0ULL)
        return false;
    for (uint64_t i = 5ULL; i * i <= n; i += 6ULL) {
        if (n % i == 0ULL || n % (i + 2ULL) == 0ULL)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* offset starts at 1 – undefined behaviour otherwise */

    static seq_t *cache = NULL;          /* cached sequence values          */
    static size_t cap   = 0;             /* current capacity of the cache   */
    static size_t len   = 0;             /* number of stored elements       */
    static idx_t next_k = 1;             /* next k to test                  */

    /* Allocate initial capacity if needed. */
    if (cache == NULL) {
        cap   = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;                  /* unlikely – allocation failure  */
    }

    /* Grow the cache until it contains at least n entries. */
    while (len < (size_t)n) {
        if (len == cap) {                    /* double the capacity when full   */
            cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;              /* unlikely – realloc failure     */
            cache = tmp;
        }

        /* Test whether 9*next_k - 5 is prime. */
        uint64_t cand = next_k * 9ULL - 5ULL;
        if (is_prime_uint64(cand)) {
            cache[len++] = (seq_t)next_k;    /* store k in the sequence cache  */
        }
        ++next_k;                              /* advance to the next k           */
    }

    return cache[n - 1];                     /* a(1) is at index 0 of the cache */
}

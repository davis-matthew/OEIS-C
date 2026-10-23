/*
 * A152087  Primes p such that q - p is not squarefree, where q is the next
 *          prime immediately following p.
 * https://oeis.org/A152087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_squarefree, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152087: primes p where the difference to the next prime q is not squarefree.
   The function searches primes sequentially and caches the qualifying primes. */

/* Required utilities */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_squarefree.h"

/* Cache for already found terms */
static seq_t *a_cache = NULL;   /* array of terms */
static size_t a_cache_len = 0;  /* number of terms stored */
static size_t a_cache_cap = 0;  /* capacity of the array */

/* The last prime that has been examined */
static uint64_t last_checked_prime = 2;

/* Append a prime to the cache, growing the array if necessary */
static void a_append(uint64_t prime)
{
    if (a_cache_len == a_cache_cap) {
        size_t grow = a_cache_cap == 0 ? 16 : a_cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(a_cache, grow * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_cache_cap = grow;
    }
    a_cache[a_cache_len++] = (seq_t)prime;
}

/* Ensure that at least n terms are computed and stored in the cache. */
static void a_ensure(idx_t n)
{
    uint64_t cur = last_checked_prime;

    while (a_cache_len < (size_t)n) {
        uint64_t next = next_prime(cur + 1);  /* next prime after cur */
        if (next == 0) break;                 /* safety guard, should not happen */

        uint64_t diff = next - cur;
        if (!is_squarefree((seq_t)diff))
            a_append(cur);                    /* cur satisfies the condition */

        cur = next;                           /* move to the next prime for inspection */
    }
    last_checked_prime = cur;                 /* remember last examined prime */
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1: undefined below 1 */
    a_ensure(n);
    return a_cache[n - 1];
}

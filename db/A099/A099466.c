/*
 * A099466  Numbers n such that 3*prime(n) + prime(n+1) - 3 is prime.
 * https://oeis.org/A099466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 542, u32 n <= 543, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, get_prime, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A099466: numbers n such that 3*prime(n)+prime(n+1)-3 is prime. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/alloc_seq_array.h"

/* Cache of sequence values: cache[i] holds a(i+1). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_check = 1;     /* last integer tested; start before 2 */

/* Allocate initial cache if needed. */
static void init_cache(void)
{
    if (cache == NULL) {
        cache_cap = 8;
        cache = alloc_seq_array(cache_cap);
    }
}

/* Return the nth term of A099466 (a(1) = 2).  Works for any call order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined; return 0 for safety */

    init_cache();

    /* Ensure the buffer can hold at least n elements. */
    if ((size_t)n > cache_cap) {
        grow_seq(&cache, &cache_cap, n);
    }

    /* Generate terms until we have n of them. */
    while (cache_len < (size_t)n) {
        next_check++;                     /* candidate integer */

        /* Obtain the n-th and (n+1)-th primes. */
        seq_t p1 = (seq_t)get_prime(next_check);
        seq_t p2 = (seq_t)get_prime(next_check + 1);

        /* Compute 3*prime(n)+prime(n+1)-3. */
        seq_t val = 3 * p1 + p2 - 3;

        /* If the result is prime, store the candidate integer. */
        if (is_prime_big(val)) {
            if (cache_len == cache_cap) {
                grow_seq(&cache, &cache_cap, cache_len + 1);
            }
            cache[cache_len++] = next_check;
        }
    }

    return cache[n - 1];
}

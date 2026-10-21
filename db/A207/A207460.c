/*
 * A207460  Let a(1) = 4. For n > 1, a(n) is the least number greater than
 *          a(n-1) such that the Hamming distance D(a(n-1),a(n)) = 4.
 * https://oeis.org/A207460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207460: a(1)=4; for n>1, a(n) is the least integer > a(n-1)
   whose binary representation differs from that of a(n-1) in exactly 4 bits. */
/* Computation by incremental search with memoization. */

#include <stdlib.h>
#include <stdbool.h>

/* Generic popcount for seq_t, works for native and arbitrary‑precision types */
static unsigned int popcnt_seq(seq_t x)
{
    unsigned int c = 0;
    while (x) {
        c += (unsigned int)(x & 1);
        x >>= 1;
    }
    return c;
}

/* Search for the smallest integer > prev with Hamming distance 4 */
static seq_t next_hamming4(seq_t prev)
{
    seq_t cand = prev + 1;
    while (popcnt_seq(prev ^ cand) != 4)
        cand += 1;
    return cand;
}

/* Cached sequence values */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Ensure capacity of cache array */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < need)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    /* n is 1‑based */
    if (n == 0) /* Undefined, return 0 for safety */
        return (seq_t)0;

    /* Handle the first term explicitly */
    if (n == 1)
        return (seq_t)4;

    /* Compute terms lazily up to n */
    if (cache_len < (size_t)n) {
        ensure_cache_cap((size_t)n);
        /* Initialize cache if empty */
        if (cache_len == 0) {
            cache[0] = (seq_t)4;
            cache_len = 1;
        }
        const seq_t *prev_ptr = &cache[cache_len - 1];
        seq_t prev = *prev_ptr;
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            seq_t next = next_hamming4(prev);
            cache[i] = next;
            prev = next;
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}

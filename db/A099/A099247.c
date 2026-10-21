/*
 * A099247  Numbers such that, in binary representation, the length and the
 *          number of ones are coprime.
 * https://oeis.org/A099247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer m such that gcd(bitlen(m),
 *            popcount(m)) = 1. (bitlen is the binary length of m, popcount is
 *            the number of 1‐bits.)
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


/* Euclidean algorithm for 64‑bit unsigned integers. */
static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Count the number of 1‑bits in a 64‑bit integer. */
static inline uint64_t popcount64(uint64_t v)
{
    uint64_t c = 0;
    while (v) {
        v &= v - 1;
        ++c;
    }
    return c;
}

/* Compute the length of the binary representation of a 64‑bit integer (>0). */
static inline uint64_t bitlen64(uint64_t v)
{
    uint64_t len = 0;
    while (v) {
        ++len;
        v >>= 1;
    }
    return len;
}

/* Return the n-th term of sequence A099247. */
seq_t a(idx_t n)
{
    /* Static cache for already computed terms. */
    static seq_t *cache = NULL;          /* array of seq_t values */
    static idx_t cache_cap = 0;          /* allocated capacity */
    static idx_t cache_len = 0;          /* number of stored terms */
    static uint64_t next_candidate = 1;  /* next integer to test */

    if (n == 0) return 0;  /* offset is 1, but caller should not request 0 */

    /* Extend the cache until we have at least n terms. */
    while (cache_len < n) {
        uint64_t val = next_candidate++;  /* candidate integer */

        uint64_t len  = bitlen64(val);
        uint64_t ones = popcount64(val);

        /* Check coprimality. */
        if (gcd64(len, ones) == 1) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                idx_t new_cap = cache_cap ? (cache_cap << 1) : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);          /* abort on allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)val;  /* store the term */
        }
    }

    /* Sequence offset is 1. */
    return cache[n - 1];
}

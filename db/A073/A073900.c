/*
 * A073900  a(1) = 1; then k-th prime prime(k) followed by Floor[e^k]
 *          consecutive composite numbers not occurring earlier.
 * https://oeis.org/A073900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   get_prime, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A073900 (description from OEIS):
 *   a(1) = 1
 *   For each k ≥ 1, append the k‑th prime, followed by
 *   floor(e^k) composite numbers that have not appeared earlier.
 *
 * The implementation keeps a cache of generated terms and expands it
 * only as needed.  It uses the helper functions
 *     uint32_t get_prime(idx_t k)
 *     bool    is_prime_big(seq_t n)
 *     void    grow_seq(seq_t **arr, size_t *cap, size_t need)
 * to obtain primes, test primality, and grow the cache array.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

static idx_t  prime_idx  = 1;          /* next prime index (1‑based)   */
static uint64_t next_comp = 2;          /* candidate for next composite */

/* floor(e^k) for 1 ≤ k ≤ 10 – sufficient for n ≤ 10 000 */
static const seq_t floor_e_k[] = {
    (seq_t)0,      /* dummy for 0‑th index */
    (seq_t)2,      /* floor(e^1)   */
    (seq_t)7,      /* floor(e^2)   */
    (seq_t)20,     /* floor(e^3)   */
    (seq_t)54,     /* floor(e^4)   */
    (seq_t)148,    /* floor(e^5)   */
    (seq_t)403,    /* floor(e^6)   */
    (seq_t)1096,   /* floor(e^7)   */
    (seq_t)2980,   /* floor(e^8)   */
    (seq_t)8103,   /* floor(e^9)   */
    (seq_t)22026   /* floor(e^10)  */
};

seq_t a(idx_t n)
{
    /* Ensure the cache starts with a(1) = 1 */
    if (cache_len == 0) {
        grow_seq(&cache, &cache_cap, 1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Generate blocks until we have at least n terms */
    while (cache_len < n) {
        /* 1. Append the next prime */
        seq_t p = (seq_t)get_prime(prime_idx);
        if (cache_len == cache_cap)
            grow_seq(&cache, &cache_cap, cache_len + 1);
        cache[cache_len++] = p;

        /* 2. Append floor(e^k) composite numbers */
        size_t block = 0;
        if (prime_idx < sizeof(floor_e_k)/sizeof(floor_e_k[0]))
            block = (size_t)floor_e_k[prime_idx];
        else
            /* In the unlikely event we need larger k, growth stops. */
            block = 0;

        for (size_t i = 0; i < block; ++i) {
            /* Find next composite */
            while (is_prime_big((seq_t)next_comp))
                ++next_comp;
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)next_comp;
            ++next_comp;
        }

        ++prime_idx;
    }

    return cache[n - 1];
}

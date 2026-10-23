/*
 * A124658  Even numbers k such that if a person is born in year k and lives
 *          not more than 100 years, then he never celebrates his prime
 *          birthday on a prime year.
 * https://oeis.org/A124658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A124658: even numbers k such that for every prime p ≤ 100,
   k + p is composite (i.e., not prime).  The sequence is generated on
   demand and cached. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* List of primes ≤ 100 (used for the check). */
static const uint32_t primes_upto_100[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31,
    37, 41, 43, 47, 53, 59, 61, 67, 71, 73,
    79, 83, 89, 97
};
static const size_t primes_upto_100_len =
    sizeof(primes_upto_100) / sizeof(primes_upto_100[0]);

/* Cached sequence values. */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

/* Next even candidate to test. */
static uint64_t next_k_candidate = 2;

/* Ensure capacity for the cache. */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 32;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();           /* fatal */
    seq_cache = new_arr;
    seq_cap   = new_cap;
}

/* Append a value to the cache. */
static void append_to_cache(seq_t val)
{
    ensure_seq_capacity(seq_len + 1);
    seq_cache[seq_len++] = val;
}

/* Grow the cache until we have at least n terms. */
static void grow_cache_to(idx_t n)
{
    while (seq_len < (size_t)n)
    {
        bool good = true;
        for (size_t i = 0; i < primes_upto_100_len; ++i)
        {
            uint32_t p = primes_upto_100[i];
            seq_t candidate = (seq_t)next_k_candidate + (seq_t)p;
            if (isPrimeGeneric(candidate)) { good = false; break; }
        }
        if (good) append_to_cache((seq_t)next_k_candidate);
        next_k_candidate += 2;           /* next even number */
    }
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but safe */
    grow_cache_to(n);
    return seq_cache[n - 1];
}

/*
 * A064736  a(1)=1, a(2)=2; for n>0, a(2*n+2) = smallest number missing from
 *          {a(1), ... ,a(2*n)}, and a(2*n+1) = a(2*n)*a(2*n+2).
 * https://oeis.org/A064736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1)=1, a(2)=2; for n>0, a(2*n+2) is the smallest positive
 *            integer not appearing in {a(1),...,a(2*n)};
 *            a(2*n+1)=a(2*n)*a(2*n+2)
 * Range:     u8 n <= 26, u16 n <= 486, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ------------ cache for already computed values ---------------- */
static seq_t *cache      = NULL;   /* 1‑based array of a(k) */
static size_t cache_cap  = 0;      /* capacity in elements */

/* ------------ bitset for used numbers (positive integers) ---- */
static uint8_t *used_bits = NULL;   /* one bit per integer value */
static size_t  used_cap   = 0;      /* capacity in bytes */

/* ------------ bookkeeping for sequence generation ------------- */
static idx_t   next_missing;      /* candidate for next even value */
static idx_t   last_even_idx;     /* index of last even term computed */
static seq_t   last_even_val;     /* value of last even term */
static bool    initialized;       /* has init_sequence() been run? */

/* -----------------------------------------------------------------
   Ensure the cache has room for index n (1‑based). 
   ----------------------------------------------------------------- */
static inline void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    cache = new_arr;
    cache_cap = new_cap;
}

/* -----------------------------------------------------------------
   Ensure the bitset has room for value v. 
   ----------------------------------------------------------------- */
static inline void ensure_used_capacity(idx_t v)
{
    size_t need_bytes = (v >> 3) + 1;
    if (need_bytes > used_cap) {
        size_t new_cap = used_cap ? used_cap << 1 : 16;
        if (new_cap < need_bytes) new_cap = need_bytes;
        uint8_t *new_arr = (uint8_t *)realloc(used_bits, new_cap);
        if (!new_arr) abort();
        if (new_cap > used_cap) memset(new_arr + used_cap, 0, new_cap - used_cap);
        used_bits = new_arr;
        used_cap = new_cap;
    }
}

/* -----------------------------------------------------------------
   Mark value v as used. 
   ----------------------------------------------------------------- */
static inline void set_used(idx_t v)
{
    ensure_used_capacity(v);
    used_bits[v >> 3] |= (uint8_t)(1 << (v & 7));
}

/* -----------------------------------------------------------------
   Test whether value v has been used. 
   ----------------------------------------------------------------- */
static inline bool is_used(idx_t v)
{
    if ((v >> 3) >= used_cap) return false;
    return (used_bits[v >> 3] >> (v & 7)) & 1U;
}

/* -----------------------------------------------------------------
   Initialise the sequence data structures. 
   ----------------------------------------------------------------- */
static void init_sequence(void)
{
    if (initialized) return;
    ensure_cache(2);
    cache[0] = 0;          /* unused */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    set_used(1);
    set_used(2);

    next_missing     = 3;          /* smallest positive integer not yet used */
    last_even_idx    = 2;          /* index of last even term (a(2)) */
    last_even_val    = (seq_t)2;   /* value of last even term (2) */
    initialized      = true;
}

/* -----------------------------------------------------------------
   Return a(n) for any n >= 1.  The function is thread‑unsafe
   but matches the required interface. 
   ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; value for n=0 is irrelevant */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    if (!initialized) init_sequence();

    /* We need all even indices up to the next even index
       that is >= n. */
    idx_t target_even = (n % 2 == 0) ? n : n + 1;

    while (last_even_idx < target_even) {
        /* --- compute new even term --- */
        idx_t new_even_idx = last_even_idx + 2;

        /* Find the smallest number not yet used.  next_missing
           always points to a candidate, we advance until it is unused. */
        while (is_used(next_missing)) next_missing++;
        seq_t new_even_val = (seq_t)next_missing;
        set_used((idx_t)new_even_val);
        ++next_missing;          /* next candidate after selecting the value */

        /* Ensure cache can hold the new even term */
        ensure_cache(new_even_idx);
        cache[new_even_idx] = new_even_val;

        /* --- compute the corresponding odd term a(new_even_idx-1) --- */
        /* Compute product in a wide type to avoid truncation during
           the test that this product fits into seq_t. */
        unsigned __int128 prod128 = (unsigned __int128)last_even_val *
                                    (unsigned __int128)new_even_val;
        seq_t odd_val = (seq_t)prod128;
        /* Only mark in the bitset if the product fits into seq_t
           (i.e. no overflow for the intended value). */
        if (prod128 <= (unsigned __int128)~(seq_t)0) {
            set_used((idx_t)odd_val);
        }
        cache[new_even_idx - 1] = odd_val;

        /* --- prepare for next iteration --- */
        last_even_idx = new_even_idx;
        last_even_val = new_even_val;
    }

    return cache[n];
}

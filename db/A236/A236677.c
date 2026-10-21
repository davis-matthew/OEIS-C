/*
 * A236677  a(0)=1; for n>0, a(n) = (1-a(floor(log_2(n)))) * a(n-msb(n));
 *          characteristic function of A079599.
 * https://oeis.org/A236677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A236677
   Formula: a(0) = 1;
   for n > 0, a(n) = (1 - a(floor(log_2(n)))) * a(n - 2^floor(log_2(n)))
   i.e. a(n) = (a[log]==0 ? a[n-2^log] : 0)                               */
#include <stdlib.h>
#include <string.h>
#include "../utils/floor_log2_uint64.h"

static seq_t *seq_cache = NULL;
static size_t cache_len = 0;   /* number of entries already computed       */
static size_t cache_cap = 0;   /* capacity of the allocated array           */

/* Ensure the cache holds indices 0..n inclusive.                    */
static void ensure_cache_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                            /* allocation failure */
    if (!seq_cache) {
        memset(tmp, 0, new_cap * sizeof(seq_t));   /* first allocation */
    } else {
        memset(tmp + cache_cap, 0,
               (new_cap - cache_cap) * sizeof(seq_t));
    }
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(i) for all i from cache_len to n (inclusive).          */
static void compute_up_to(idx_t n)
{
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            seq_cache[0] = (seq_t)1;
        } else {
            unsigned log = floor_log2_uint64(i);      /* floor(log2(i))   */
            idx_t msb   = ((idx_t)1) << log;          /* 2^log           */
            idx_t rest  = i - msb;                    /* n without msb   */
            seq_cache[i] = (seq_cache[log] == 0 ?
                            seq_cache[rest] : (seq_t)0);
        }
    }
    cache_len = (size_t)n + 1;
}

/* Public entry point                                  */
seq_t a(idx_t n)
{
    if (n >= cache_len) {
        ensure_cache_capacity(n);
        compute_up_to(n);
    }
    return seq_cache[n];
}

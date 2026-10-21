/*
 * A201874  Number of zero-sum -n..n arrays of 3 elements with first and
 *          second differences also in -n..n.
 * https://oeis.org/A201874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A201874: Empirical recurrence
   a(n) = a(n-1) + 2*a(n-3) - 2*a(n-4) - a(n-6) + a(n-7)
   Initial values: a(1)=3, a(2)=5, a(3)=15, a(4)=21, a(5)=27,
                    a(6)=47, a(7)=57.
   Computed on demand, caches results for later calls. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *seq_cache = NULL;   /* 1‑based indexing, seq_cache[0] unused */
static idx_t cache_len = 0;       /* highest n computed (>=7 after init) */
static idx_t cache_cap = 0;       /* allocated capacity (0 means not yet allocated) */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if (n <= cache_cap)
        return;
    idx_t new_cap = (cache_cap > 0) ? cache_cap : 8;
    while (new_cap < n)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, (new_cap + 1) * sizeof(seq_t));
    seq_cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise cache with the first seven terms. */
static void init_cache(void)
{
    if (cache_len != 0)
        return;
    ensure_cache_size(8);
    seq_cache[1] = (seq_t)3;
    seq_cache[2] = (seq_t)5;
    seq_cache[3] = (seq_t)15;
    seq_cache[4] = (seq_t)21;
    seq_cache[5] = (seq_t)27;
    seq_cache[6] = (seq_t)47;
    seq_cache[7] = (seq_t)57;
    cache_len = 7;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;         /* out of range, but safe */
    init_cache();

    if (n > (idx_t)cache_len) {
        ensure_cache_size(n);
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            if (k <= 7)    /* already initialised in init_cache */
                continue;
            seq_t val = seq_cache[k - 1];
            val += ((seq_t)2) * seq_cache[k - 3];
            val += seq_cache[k - 7];
            val -= ((seq_t)2) * seq_cache[k - 4];
            val -= seq_cache[k - 6];
            seq_cache[k] = val;
        }
        cache_len = n;
    }
    return seq_cache[n];
}

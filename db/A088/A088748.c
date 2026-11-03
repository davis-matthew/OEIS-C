/*
 * A088748  a(n) = 1 + Sum_{k=0..n-1} 2 * A014577(k) - 1.
 * https://oeis.org/A088748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=0}^{n-1} (2*A014577(k) - 1) A014577 is the
 *            paperfolding sequence: f(0) = 1 f(2m) = f(m) f(4m+1) = 1 f(4m+3)
 *            = 0 In a(n), every step is +1 if the next paperfolding value is
 *            1, and –1 if it is 0
 * Range:     u8 n <= 16383, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 118 terms through n = 9951 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute bit value of the paperfolding sequence (0/1).               */
static int paperfolding_bit(idx_t n)
{
    if (n == 0) return 1;          /* f(0) = 1 */
    /* reduce n until it is odd */
    while ((n & 1ULL) == 0)
        n >>= 1;
    /* now n is odd: it is 1 mod 4 or 3 mod 4 */
    return ((n & 3ULL) == 1) ? 1 : 0;
}

/* Difference between successive terms: +1 if bit is 1, -1 otherwise. */
static inline int diff_of(idx_t k)
{
    /* use bit value of k+1 according to the recurrence above. */
    return paperfolding_bit(k + 1) ? 1 : -1;
}

/* Cache for already computed terms of the sequence. */
static seq_t *cache = NULL;    /* array of a(i) values */
static size_t cache_cap = 0;   /* current allocated capacity (in elements) */
static idx_t  cache_len = 0;   /* number of values stored (last index + 1) */

seq_t a(idx_t n)
{
    /* Allocate and init cache if necessary. */
    if (!cache) {
        cache_cap = 1;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return (seq_t)0;           /* allocation failure */
        cache[0] = (seq_t)1;                  /* a(0) = 1 */
        cache_len = 1;
    }

    /* Grow cache until it contains a(n). */
    while (cache_len <= n) {
        /* Ensure enough capacity. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) return (seq_t)0;        /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute a(cache_len) from a(cache_len-1). */
        idx_t k = cache_len - 1;              /* previous index */
        signed long long cur = (long long)cache[cache_len - 1];
        cur += diff_of(k);                    /* add +1 or -1 */
        cache[cache_len] = (seq_t)cur;        /* store new term */
        ++cache_len;
    }

    return cache[n];
}

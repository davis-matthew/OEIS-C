/*
 * A176941  Primes of the form 31^n + 10^n.
 * https://oeis.org/A176941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref, more
 * Formula:   a(n) = 31^m + 10^m where m = 2^(n-2) for n ≥ 2, a(1) = 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   grow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence returns the nth prime of the form 31^k + 10^k.
   The only exponents k that can produce primes are powers of 2
   (k = 0,1,2,4,8,…) so we generate the terms incrementally by
   squaring each previous 31^k and 10^k. This gives O(n) work for the
   nth term. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"          /* seq_mul */
#include "../utils/grow_seq.h"         /* grow_seq */

static seq_t *cache = NULL;            /* cached terms */
static size_t cache_len = 0;           /* number of cached terms */
static size_t cache_cap = 0;           /* capacity of cache array */
static seq_t curr31 = 0;               /* current 31^(2^m) */
static seq_t curr10 = 0;               /* current 10^(2^m) */
static bool init_done = false;         /* initialization flag */

/* Compute the nth term of the sequence */
seq_t a(idx_t n)
{
    /* n is >= 1 (offset 1) */
    if (n == 0) return 0;  /* safety guard – not used */

    /* Initialise the running powers once */
    if (!init_done) {
        curr31 = 31;
        curr10 = 10;
        init_done = true;
    }

    /* Grow the cache if needed */
    if ((size_t)n > cache_cap) {
        grow_seq(&cache, &cache_cap, (size_t)n);
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            /* k = 0: 31^0 + 10^0 = 2 */
            val = 2;
        } else if (i == 2) {
            /* k = 1: curr31 = 31, curr10 = 10 */
            val = curr31 + curr10;
        } else {
            /* For i > 2, k = 2^(i-2); square the previous powers */
            curr31 = seq_mul(curr31, curr31);
            curr10 = seq_mul(curr10, curr10);
            val = curr31 + curr10;
        }
        cache[i - 1] = val;
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
